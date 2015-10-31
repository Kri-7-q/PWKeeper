#include "persistence.h"
#include "credentials.h"
#include "tablemodel.h"
#include <QSqlField>

/**
 * Constructor
 */
Persistence::Persistence() :
    m_primaryKey(QString("id")),
    m_uniqueKey(QStringList() << QString("provider") << QString("username")),
    m_tableName("accountlist")
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "local");
    initializeDatabase(db);
    if (db.open()) {
        m_record = db.record(m_tableName);
        db.close();
    }
}

/**
 * Public
 * Open database.
 * @return
 */
QSqlDatabase Persistence::openDatabase() const
{
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    if (! db.open()) {
        throw SqlException(QString("Could not open database !"));
    }

    return db;
}

/**
 * Close database.
 * @param db        The database to close.
 */
void Persistence::closeDatabase(QSqlDatabase &db) const
{
    db.close();
}

/**
 * Persists a new Account object.
 * @param account       The new account object.
 * @param db            A database object reference. Database must be open.
 * @return result       True if object was stored.
 */
bool Persistence::persistAccountObject(const QVariantMap &account, const QSqlDatabase &db) const
{
    QSqlRecord insertRecord = recordFromVariantMap(account);
    QString insertSql = db.driver()->sqlStatement(QSqlDriver::InsertStatement, m_tableName, insertRecord, true);
    QSqlQuery query(db);
    bool result = query.prepare(insertSql);
    if (!result) {
        SqlException exception(QString("Persistence:persistAccount() --> Could not prepare statement !"), insertSql, query.lastError().databaseText());
        throw exception;
    }
    for (int index=0; index<insertRecord.count(); ++index) {
        query.addBindValue(insertRecord.value(index));
    }
    result = query.exec();
    if (! result) {
        SqlException exception(QString("Persistence:persistAccount() --> Could not execute query !"), insertSql, query.lastError().databaseText());
        throw exception;
    }

    return true;
}

/**
 * Public
 * Delete an Account from database using primary key.
 * @param objectId      The id of an Account object. (Primary Key)
 * @param db            A reference to an open database.
 * @return              True if Account was deleted.
 */
bool Persistence::deleteAccountObject(const int objectId, const QSqlDatabase &db) const
{
    QString deleteSql = QString("DELETE FROM %1 WHERE id=?").arg(m_tableName);
    QSqlQuery query(db);
    bool result = query.prepare(deleteSql);
    if (! result) {
        SqlException exception(QString("Persistance:deleteAccount() --> Could not prepare query !"), deleteSql, query.lastError().databaseText());
        throw exception;
    }
    query.addBindValue(QVariant(objectId));
    result = query.exec();
    if (! result) {
        SqlException exception(QString("Persistence:deleteAccount() --> Could not execute query !"), deleteSql, query.lastError().databaseText());
        throw exception;
    }

    return true;
}

/**
 * Modify an existing Account object.
 * @param id                The objects id.
 * @param modifications     All values which are to update.
 * @param db                A reference to an open datbase.
 * @return                  True if Account was updated.
 */
bool Persistence::modifyAccountObject(const int id, const QVariantMap &modifications, const QSqlDatabase &db) const
{
    QSqlRecord modifyRecord = recordFromVariantMap(modifications);
    QString modifySql = db.driver()->sqlStatement(QSqlDriver::UpdateStatement, m_tableName, modifyRecord, true);
    modifySql.append(QString(" WHERE id=?"));
    QSqlQuery query(db);
    bool result = query.prepare(modifySql);
    if (! result) {
        SqlException exception(QString("Persistence:modifyAccountObject() --> Could not prepare statement !"), modifySql, query.lastError().databaseText());
        throw exception;
    }
    for (int index=0; index<modifyRecord.count(); ++index) {
        query.addBindValue(modifyRecord.value(index));
    }
    query.addBindValue(QVariant(id));
    result = query.exec();
    if (! result) {
        SqlException exception(QString("Persistence:modifyAccountObject() --> Could not execute prepared query !"), modifySql, query.lastError().databaseText());
        throw exception;
    }

    return true;
}

/**
 * Find an Account by is primary key the id value.
 * @param id        The id of an Account.
 * @param db        A reference to an open database.
 * @return          An Account object or an empty QVariantMap.
 */
QVariantMap Persistence::findAccount(const int id, const QSqlDatabase &db) const
{
    QString selectSql = QString("SELECT * FROM %1 WHERE id=?").arg(m_tableName);
    QSqlQuery query(db);
    bool result = query.prepare(selectSql);
    if (! result) {
        SqlException exception(QString("Persistence:findAccount(int) --> Could not prepare query !"), selectSql, query.lastError().databaseText());
        throw exception;
    }
    query.addBindValue(QVariant(id));
    result = query.exec();
    if (! result) {
        SqlException exception(QString("Persistence:findAccount(int) --> Could not execute query !"), selectSql, query.lastError().databaseText());
        throw exception;
    }
    if (! query.next()) {
        return QVariantMap();
    }

    return getAccountObject(query, query.record());
}

/**
 * Find an account by unique constraint.
 * Constraint consists of provider name and username.
 * @param providerName      The provider name.
 * @param username          The username.
 * @return                  A map with an Account object.
 */
QVariantMap Persistence::findAccount(const QString &providerName, const QString &username, const QSqlDatabase& db) const
{
    QString selectSql = QString("SELECT * FROM %1 WHERE provider=? AND username=?").arg(m_tableName);
    QSqlQuery query(db);
    bool result = query.prepare(selectSql);
    if (! result) {
        SqlException exception(QString("Persistence:FindAccount() --> Could not prepare query !"), selectSql, query.lastError().databaseText());
        throw exception;
    }
    query.addBindValue(QVariant(providerName));
    query.addBindValue(QVariant(username));
    result = query.exec();
    if (! result) {
        SqlException exception(QString("Persistence:findAccount() --> Could not execute query !"), selectSql, query.lastError().databaseText());
        throw exception;
    }
    if (! query.next()) {
        return QVariantMap();
    }

    return getAccountObject(query, query.record());
}

/**
 * Takes a QVariantMap with parameters. The map should use database column names as key.
 * Every column which should be read must exist as a key in the map. If no value is to
 * search in column set a empty QVariant() as value. If there is to search for a value
 * in a column than set the search value to the key (column name).
 * @param searchObject      A map with column names as key an values to search for.
 * @return accountList      A list with Account objects (QVariantMap).
 */
QList<QVariantMap> Persistence::findAccounts(const QVariantMap &searchObject) const
{
    QSqlDatabase db = openDatabase();
    QSqlRecord selectRecord = recordFromVariantMap(searchObject);
    QSqlRecord whereClauseRecord = recordFieldsWithValuesFromRecord(selectRecord);
    QString selectSql = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, selectRecord, false);
    QString whereClauseSql = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereClauseRecord, true);
    if (! whereClauseSql.isEmpty()) {
        selectSql.append(' ').append(whereClauseSql);
    }
    QSqlQuery query(db);
    bool result = query.prepare(selectSql);
    if (! result) {
        SqlException exception(QString("Persistence:findAccounts() --> Could not prepare statement !"), selectSql, query.lastError().databaseText());
        throw exception;
    }
    for (int index=0; index<whereClauseRecord.count(); ++index) {
        query.addBindValue(whereClauseRecord.value(index));
    }
    result = query.exec();
    if (! result) {
        SqlException exception(QString("Persistence:findAccounts() --> Could not execute query !"), selectSql, query.lastError().databaseText());
        throw exception;
    }
    QList<QVariantMap> accountList = getAccountList(query);
    db.close();

    return accountList;
}

/**
 * Get all column names of a database table.
 * Column names are read from database.
 * @param tableName         The name of a database table.
 * @return columnList       A list of all column names.
 */
QStringList Persistence::getColumnNames(const QString tableName) const
{
    QStringList columnList;
    for (int index=0; index<m_record.count(); ++index) {
        columnList << m_record.fieldName(index);
    }

    return columnList;
}

/**
 * Read all content of a table.
 * @param tableName         The name of the database table.
 * @return accountList      A list with all table content.
 */
QList<QVariantMap> Persistence::readWholeTable(const QString tableName) const
{
    QSqlDatabase db = openDatabase();
    QString selectSql = QString("SELECT * FROM %1").arg(tableName);
    QSqlQuery query = db.exec(selectSql);
    if (!query.isValid() && query.lastError().isValid()) {
        SqlException exception(QString("Could not read from database !"), selectSql, query.lastError().databaseText());
        exception.setDatabaseError(query.lastError().driverText());
        exception.setSqlStatement(selectSql);
        throw exception;
    }
    QList<QVariantMap> accountList = getAccountList(query);
    db.close();

    return accountList;
}

/**
 * Get data type info of database columns.
 * Data type is a QVariant::Type and store in QHash as value.
 * Column names are the keys to the values in QHash.
 * @return      A hash map with data type info to database columns.
 */
QHash<QString, QVariant::Type> Persistence::getTablesDataTypes() const
{
    if (m_record.isEmpty()) {
        throw SqlException(QString("Persistence:getTableDataTypes() --> No table info avalable !"),
                           QString(), QString());
    }
    QHash<QString, QVariant::Type> dataTypeMap;
    for (int index=0; index<m_record.count(); ++index) {
        QSqlField field = m_record.field(index);
        QString columnName = field.name();
        QVariant::Type dataType = field.type();
        dataTypeMap.insert(columnName, dataType);
    }

    return dataTypeMap;
}

/**
 * Public static
 * Create a QHash<int, QByteArray> map with TableModel roles as key
 * and database column names as values.
 * The column names are used as role names.
 * @return roles    A map with roles.
 */
QHash<int, QByteArray> Persistence::getTableModelRoles()
{
    QHash<int, QByteArray> roles;
    roles.insert(TableModel::IdRole, QString("id").toLocal8Bit());
    roles.insert(TableModel::ProviderRole, QString("provider").toLocal8Bit());
    roles.insert(TableModel::UsernameRole, QString("username").toLocal8Bit());
    roles.insert(TableModel::PasswordRole, QString("password").toLocal8Bit());
    roles.insert(TableModel::QuestionRole, QString("question").toLocal8Bit());
    roles.insert(TableModel::AnswerRole, QString("answer").toLocal8Bit());
    roles.insert(TableModel::LengthRole, QString("passwordlength").toLocal8Bit());
    roles.insert(TableModel::DefinedCharacterRole, QString("definedcharacter").toLocal8Bit());
    roles.insert(TableModel::LastModifyRole, QString("lastmodify").toLocal8Bit());

    return roles;
}

/**
 * Creates a QSqlRecord object from a QVariantMap cotaining column names as key.
 * @param searchObject      A Map with column names as key and QVariant values.
 * @return record           A record with columns and values of searchOject.
 */
QSqlRecord Persistence::recordFromVariantMap(const QVariantMap &searchObject) const
{
    QSqlRecord record;
    QStringList columnNameList = searchObject.keys();
    foreach (QString columnName, columnNameList) {
        QSqlField field = m_record.field(columnName);
        QVariant value = searchObject.value(columnName);
        if (value.isValid()) {
            field.setValue(value);
        }
        record.append(field);
    }

    return record;
}

/**
 * Extracts fields with values from a record.
 * @param sourceRecord      The record with fields to extract.
 * @return record           A record with fields containing a value.
 */
QSqlRecord Persistence::recordFieldsWithValuesFromRecord(const QSqlRecord &sourceRecord) const
{
    QSqlRecord record;
    for (int index=0; index<sourceRecord.count(); ++index) {
        QSqlField field = sourceRecord.field(index);
        if (! field.isNull()) {
            record.append(field);
        }
    }

    return record;
}


/**
 * Takes a QSqlQuery object after query was executed. Builds a list with
 * QVariantMap objects. The QVariantMap objects contain the data with
 * the database column names as key.
 * @param query     A executed QSqlQuery object.
 * @return list     A list containing QVariantMap objects.
 */
QList<QVariantMap> Persistence::getAccountList(QSqlQuery &query) const
{
    QList<QVariantMap> list;
    QSqlRecord record = query.record();
    while (query.next()) {
        list << getAccountObject(query, record);
    }

    return list;
}

/**
 * Get an Account object out of a QSqlQuery object.
 * @param query     A query where next() was called.
 * @return account  A map with an Account object.
 */
QVariantMap Persistence::getAccountObject(const QSqlQuery &query, const QSqlRecord& record) const
{
    QVariantMap account;
    for (int i=0; i<record.count(); ++i) {
        account.insert(record.fieldName(i), query.value(i));
    }

    return account;
}

/**
 * Initialize database with Credentials from a file.
 * @param db
 */
void Persistence::initializeDatabase(QSqlDatabase &db) const
{
    QString homePath = Credentials::usersHomePath();
    QString filePath = homePath.append("/.pwmanager");
    Credentials credentials = Credentials::credentialsFromFile(filePath);
    db.setDatabaseName(credentials.value(Credentials::DatabaseName));
    db.setHostName(credentials.value(Credentials::Hostname));
    db.setUserName(credentials.value(Credentials::Username));
    db.setPassword(credentials.value(Credentials::Password));
    db.setPort(credentials.value(Credentials::Port).toInt());
}

