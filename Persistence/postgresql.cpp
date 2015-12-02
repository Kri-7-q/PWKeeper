#include "postgresql.h"
#include "credentials.h"
#include "SqlDriver/PostgreSql/kqpostgresqldriver.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <QDateTime>
#include <QDebug>

/**
 * Constructor
 */
PostgreSql::PostgreSql() :
    m_databaseIdentifier("local")
{
    initialize();
}

/**
 * Destructor
 */
PostgreSql::~PostgreSql()
{

}

/**
 * Override
 * Open a database connection.
 * @return          True if database is open.
 */
bool PostgreSql::open() const
{
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);

    return db.isOpen();
}

/**
 * Override
 * Closes an open database connection.
 */
void PostgreSql::close() const
{
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, false);
    db.close();
}

/**
 * Override
 * Persist a new Account object.
 * Add current date to the Account object as a timestamp
 * of last modification.
 * @param account       A new Account object.
 * @return              True if done. Otherwise false.
 */
bool PostgreSql::persistAccountObject(const QVariantMap &account) const
{
    QVariantMap obj = account;
    obj.insert(QString("lastmodify"), QDateTime::currentDateTime());
    QSqlRecord record = recordFromAccountObject(obj);
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QString sqlInsert = db.driver()->sqlStatement(QSqlDriver::InsertStatement, m_tableName, record, true);
    QSqlQuery query(db);
    if (! query.prepare(sqlInsert)) {
        qDebug() << "PostgreSql::persistAccountObject() --> prepare(sqlStatement)";
        qDebug() << "SQL: " << sqlInsert;
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return false;
    }
    for (int index=0; index<record.count(); ++index) {
        query.addBindValue(record.value(index));
    }

    if(! query.exec()) {
        qDebug() << "PostgreSql::persistAccountObject() --> exec()";
        qDebug() << "SQL: " << sqlInsert;
        qDebug() << "Values: " << query.boundValues();
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return false;
    }

    return true;
}

/**
 * Override
 * Delete Account object from persistence.
 * Identify object with its primary key.
 * @param primaryKey        A unique Id to identify Account object.
 * @return                  True if deleted. Otherwise false.
 */
bool PostgreSql::deleteAccountObject(const QVariantMap &account) const
{
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QString sqlDelete = db.driver()->sqlStatement(QSqlDriver::DeleteStatement, m_tableName, m_record, false);
    QSqlRecord whereRecord = recordWithIdentifier(account);
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    sqlDelete.append(' ').append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlDelete)) {
        qDebug() << "PostgreSql::deleteAccountObject() --> prepare(sqlStatement)";
        qDebug() << "SQL: " << sqlDelete;
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return false;
    }
    for (int index=0; index<whereRecord.count(); ++index) {
        query.addBindValue(whereRecord.value(index));
    }

    if(! query.exec()) {
        qDebug() << "PostgreSql::deleteAccountObject() --> exec()";
        qDebug() << "SQL: " << sqlDelete;
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return false;
    }

    return true;
}

/**
 * Override
 * Update a modified Account object in persistence.
 * Reads origin Account object from persistence. Compares
 * origin and modified. Differences between them are put
 * into an record to build a SQL update query.
 * @param modifiedObject
 * @return                  True if updated. Otherwise false.
 */
bool PostgreSql::modifyAccountObject(const QVariantMap &modifiedObject) const
{
    QVariantMap origin = findAccount(modifiedObject);
    if (origin.isEmpty()) {
        qDebug() << "PostgreSql::modifyAccountObject() --> findAccount(origin)";
        qDebug() << "Could not find the origin Account object in database.";
        return false;
    }
    QVariantMap differences = differenceToOrigin(origin, modifiedObject);
    if (differences.isEmpty()) {
        return true;
    }
    differences.insert(QString("lastmodify"), QDateTime::currentDateTime());
    QSqlRecord record = recordFromAccountObject(differences);
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QString sqlUpdate = db.driver()->sqlStatement(QSqlDriver::UpdateStatement, m_tableName, record, true);
    QSqlRecord whereRecord = recordWithIdentifier(origin);
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    sqlUpdate.append(' ').append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlUpdate)) {
        qDebug() << "PostgreSql::modifyAccountObject() --> prepare(sqlStatement)";
        qDebug() << "SQL: " << sqlUpdate;
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return false;
    }
    for (int index=0; index<record.count(); ++index) {
        query.addBindValue(record.value(index));
    }
    for (int index=0; index<whereRecord.count(); ++index) {
        query.addBindValue(whereRecord.value(index));
    }

    if(! query.exec()) {
        qDebug() << "PostgreSql::modifyAccountObject() --> exec()";
        qDebug() << "SQL: " << sqlUpdate;
        qDebug() << "Values: " << query.boundValues();
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return false;
    }

    return true;
}

/**
 * Override
 * Get an Account object from persistence by its primary key.
 * @param primaryKey        A unique value which identifies an Account object in persistence.
 * @return account          An Account object or an empty map.
 */
QVariantMap PostgreSql::findAccount(const QVariantMap &searchObj) const
{
    QVariantMap account;
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, m_record, false);
    QSqlRecord whereRecord = recordWithIdentifier(searchObj);
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    sqlSelect.append(' ').append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlSelect)) {
        qDebug() << "PostgreSql::findAccount(searchObj) --> prepare(sqlStatement)";
        qDebug() << "SQL: " << sqlSelect;
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return account;
    }
    for (int index=0; index<whereRecord.count(); ++index) {
        query.addBindValue(whereRecord.value(index));
    }
    if (! query.exec()) {
        qDebug() << "PostgreSql::findAccount(searchObj) --> exec()";
        qDebug() << "SQL: " << sqlSelect;
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return account;
    }
    if (query.next()) {
        account = getAccountObject(query.record());
    }

    return account;
}

/**
 * Override
 * Get a list of all persisted Account objects.
 * @return      A list of Account objects.
 */
QList<QVariantMap> PostgreSql::allPersistedAccounts() const
{
    QList<QVariantMap> accountList;
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    if (! db.isOpen()) {
        qDebug() << "PostgreSql::allPersistedAccounts() --> isOpen()";
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return accountList;
    }
    QSqlRecord record = db.record(m_tableName);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, record, false);
    QSqlQuery query(sqlSelect, db);
    while (query.next()) {
        accountList << getAccountObject(query.record());
    }
    if (accountList.isEmpty()) {
        qDebug() << "PostgreSql::allPersistedAccounts() --> query(sqlStatement, db)";
        qDebug() << "SQL: " << sqlSelect;
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
    }
    db.close();

    return accountList;
}

/**
 * Initializes the PostgreSql database.
 * Reads credentials from a file in home directory.
 * Set these credentials to the database.
 * Get database table name from the file too.
 */
void PostgreSql::initialize()
{
    QString homePath = Credentials::usersHomePath();
    QString filePath = homePath + QString("/.pwmanager");
    Credentials credentials = Credentials::credentialsFromFile(filePath);
    QSqlDatabase db = QSqlDatabase::addDatabase(new KQPostgreSqlDriver(), m_databaseIdentifier);
    db.setHostName(credentials.value(Credentials::Hostname));
    db.setDatabaseName(credentials.value(Credentials::DatabaseName));
    db.setPort(credentials.value(Credentials::Port).toInt());
    db.setUserName(credentials.value(Credentials::Username));
    db.setPassword(credentials.value(Credentials::Password));
    m_tableName = credentials.value(Credentials::TableName);
    if (db.open()) {
        m_record = db.record(m_tableName);
    } else {
        qDebug() << "PostgreSql::initialize() --> db.open()";
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
    }
}

/**
 * Private
 * Get a Account object (QVariantMap) from a query record.
 * @param record        A record from a executed query.
 * @return account      An Account object.
 */
QVariantMap PostgreSql::getAccountObject(const QSqlRecord &record) const
{
    QVariantMap account;
    for (int index=0; index<record.count(); ++index) {
        QString key = record.fieldName(index);
        QVariant value = record.value(index);
        account.insert(key, value);
    }

    return account;
}

/**
 * Creats a record object with database key fields for a where clause.
 * @param account
 * @return
 */
QSqlRecord PostgreSql::recordWithIdentifier(const QVariantMap &account) const
{
    QSqlRecord record;
    QSqlField field;
    QString id("id"), provider("provider"), username("username");
    if (account.contains(id)) {
        field = m_record.field(id);
        field.setValue(account.value(id));
        record.append(field);

        return record;
    }
    field = m_record.field(provider);
    field.setValue(account.value(provider));
    record.append(field);
    field = m_record.field(username);
    field.setValue(account.value(username));
    record.append(field);

    return record;
}

/**
 * Private
 * Get a record object from Account object.
 * Returned record object contains columns and values.
 * Can be used to get a SQL statement and bind values.
 * @param account
 * @return record       A record with columns and values.
 */
QSqlRecord PostgreSql::recordFromAccountObject(const QVariantMap &account) const
{
    QSqlRecord record;
    foreach (QString column, account.keys()) {
        if (m_record.contains(column)) {
            QSqlField field = m_record.field(column);
            field.setValue(account.value(column));
            record.append(field);
        }
    }

    return record;
}

/**
 * Private
 * Find modifications of modified Account object.
 * Compare origin Account with modified Account.
 * @param origin
 * @param modified
 * @return differences      A map with differences between origin and modified.
 */
QVariantMap PostgreSql::differenceToOrigin(const QVariantMap &origin, const QVariantMap &modified) const
{
    QVariantMap differences;
    foreach (QString key, modified.keys()) {
        QVariant originVal = origin.value(key);
        QVariant modifiedVal = modified.value(key);
        if (originVal != modifiedVal) {
            differences.insert(key, modifiedVal);
        }
    }

    return differences;
}

