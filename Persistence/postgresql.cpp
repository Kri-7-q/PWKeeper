#include "postgresql.h"
#include "credentials.h"
#include "SqlDriver/PostgreSql/kqpostgresqldriver.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <QDateTime>

/**
 * Constructor
 */
PostgreSql::PostgreSql() :
    m_databaseIdentifier("local")
{
    initialize();
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
    QSqlRecord record = getRecordFromAccountObject(obj);
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QString sqlInsert = db.driver()->sqlStatement(QSqlDriver::InsertStatement, m_tableName, record, true);
    QSqlQuery query(db);
    if (! query.prepare(sqlInsert)) {
        // ToDo: provide error message
        return false;
    }
    for (int index=0; index<record.count(); ++index) {
        query.addBindValue(record.value(index));
    }

    return query.exec();
}

/**
 * Override
 * Delete Account object from persistence.
 * Identify object with its primary key.
 * @param primaryKey        A unique Id to identify Account object.
 * @return                  True if deleted. Otherwise false.
 */
bool PostgreSql::deleteAccountObject(const QVariant &primaryKey) const
{
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QSqlRecord tableInfo = db.record(m_tableName);
    QString sqlDelete = db.driver()->sqlStatement(QSqlDriver::DeleteStatement, m_tableName, tableInfo, false);
    QSqlRecord whereRecord;
    whereRecord.append(tableInfo.field(QString("id")));
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    sqlDelete.append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlDelete)) {
        // ToDo: provide error message
        return false;
    }
    query.addBindValue(primaryKey);

    return query.exec();
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
    QVariantMap origin;
    if (modifiedObject.contains(QString("id"))) {
        origin = findAccount(modifiedObject.value(QString("id")));
    } else {
        QString provider = modifiedObject.value(QString("provider")).toString();
        QString username = modifiedObject.value(QString("username")).toString();
        origin = findAccount(provider, username);
    }
    QVariantMap differences = getModifications(origin, modifiedObject);
    if (differences.isEmpty()) {
        return true;
    }
    differences.insert(QString("lastmodify"), QDateTime::currentDateTime());
    QSqlRecord record = getRecordFromAccountObject(differences);
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QString sqlUpdate = db.driver()->sqlStatement(QSqlDriver::UpdateStatement, m_tableName, record, true);
    QSqlRecord tableInfo = db.record(m_tableName);
    QSqlField primaryKey = tableInfo.field(QString("id"));
    primaryKey.setValue(origin.value(QString("id")));
    QSqlRecord whereRecord;
    whereRecord.append(primaryKey);
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    record.append(primaryKey);
    sqlUpdate.append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlUpdate)) {
        // ToDo: provide error message
        return false;
    }
    for (int index=0; index<record.count(); index) {
        query.addBindValue(record.value(index));
    }

    return query.exec();
}

/**
 * Override
 * Get an Account object from persistence by its primary key.
 * @param primaryKey        A unique value which identifies an Account object in persistence.
 * @return account          An Account object or an empty map.
 */
QVariantMap PostgreSql::findAccount(const QVariant &primaryKey) const
{
    QVariantMap account;
    if (! open()) {
        // ToDo: provide error message
        return account;
    }
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QSqlRecord record = db.record(m_tableName);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, record, false);
    QSqlRecord whereRecord;
    whereRecord.append(record.field(QString("id")));
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    sqlSelect.append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlSelect)) {
        // ToDo: provide error message
        return account;
    }
    query.addBindValue(primaryKey);
    if (! query.exec()) {
        // ToDo: provide error message
        return account;
    }
    if (query.next()) {
        account = getAccountObject(query.record());
    }

    return account;
}

/**
 * Override
 * Read an Account object from persistence with the unique values 'provider' and 'username'.
 * @param providerName
 * @param username
 * @return                  An Account object or an empty map.
 */
QVariantMap PostgreSql::findAccount(const QString &providerName, const QString &username) const
{
    QVariantMap account;
    if (! open()) {
        // ToDo: provide error message
        return account;
    }
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QSqlRecord record = db.record(m_tableName);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, record, false);
    QSqlRecord whereRecord;
    whereRecord.append(record.field(QString("provider")));
    whereRecord.append(record.field(QString("username")));
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    sqlSelect.append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlSelect)) {
        // ToDo: provide error message
        return account;
    }
    query.addBindValue(QVariant(providerName));
    query.addBindValue(QVariant(username));
    if (! query.exec()) {
        // ToDo: provide error mesage
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
    if (! open()) {
        // ToDo:    provide error messages.
        return accountList;
    }
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QSqlRecord record = db.record(m_tableName);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, record, false);
    QSqlQuery query(sqlSelect, db);
    while (query.next()) {
        accountList << getAccountObject(query.record());
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
 * Private
 * Get a record object from Account object.
 * Returned record object contains columns and values.
 * Can be used to get a SQL statement and bind values.
 * @param account
 * @return record       A record with columns and values.
 */
QSqlRecord PostgreSql::getRecordFromAccountObject(const QVariantMap &account) const
{
    QSqlDatabase db = QSqlDatabase::database(m_databaseIdentifier, true);
    QSqlRecord tableInfo = db.record(m_tableName);
    QSqlRecord record;
    foreach (QString column, account.keys()) {
        QSqlField field = tableInfo.field(column);
        field.setValue(account.value(column));
        record.append(field);
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
QVariantMap PostgreSql::getModifications(const QVariantMap &origin, const QVariantMap &modified) const
{
    QStringList keyList = origin.keys() << modified.keys();
    QSet<QString> keySet = keyList.toSet();
    QVariantMap differences;
    foreach (QString key, keySet) {
        QVariant originVal = origin.value(key);
        QVariant modifiedVal = modified.value(key);
        if (originVal != modifiedVal) {
            differences.insert(key, modifiedVal);
        }
    }

    return differences;
}

