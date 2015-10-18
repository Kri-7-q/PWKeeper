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
    QStringList columnList = account.keys();
    QString insertSql = insertIntoSql(columnList);
    QSqlQuery query(db);
    bool result = query.prepare(insertSql);
    if (!result) {
        SqlException exception(QString("PersistAccount: Could not prepare statement !"), insertSql, query.lastError().databaseText());
        throw exception;
    }
    foreach (QString column, columnList) {
        QVariant value = account.value(column, QVariant());
        query.addBindValue(value);
    }
    result = query.exec();
    if (! result) {
        SqlException exception(QString("PersistAccount: Could not execute query !"), insertSql, query.lastError().databaseText());
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
    QString deleteSql = QString("DELETE FROM %1 WHERE id=%2").arg(m_tableName).arg(objectId);
    QSqlQuery query(db);
    bool result = query.exec(deleteSql);
    if (! result) {
        SqlException exception(QString("DeleteAccount: Could not excecute query !"), deleteSql, query.lastError().databaseText());
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
    QString modifySql = QString("UPDATE %1 SET %2 WHERE id=%3").arg(m_tableName)
                                                               .arg(updateTupleString(modifications))
                                                               .arg(id);
    QSqlQuery query(db);
    bool result = query.prepare(modifySql);
    if (! result) {
        SqlException exception(QString("modifyAccountObject: Could not prepare statement !"), modifySql, query.lastError().databaseText());
        throw exception;
    }
    foreach (QString key, modifications.keys()) {
        QString bindString = QString(':').append(key);
        QVariant value = modifications.value(key);
        query.bindValue(bindString, value);
    }
    result = query.exec();
    if (! result) {
        SqlException exception(QString("modifyAccountObject: Could not execute prpared query !"), modifySql, query.lastError().databaseText());
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
    QString selectSql = QString("SELECT * FROM %1 WHERE id=%2").arg(m_tableName).arg(id);
    QSqlQuery query(db);
    bool result = query.exec(selectSql);
    if (! result) {
        SqlException exception(QString("findAccount: Could not execute query !"), selectSql, query.lastError().databaseText());
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
    QString selectSql = QString("SELECT * FROM %1 WHERE provider='%2' AND username='%3'")
                        .arg(m_tableName)
                        .arg(providerName)
                        .arg(username);
    QSqlQuery query(db);
    bool result = query.exec(selectSql);
    if (! result) {
        SqlException exception(QString("FindAccount: Could not execute query !"), selectSql, query.lastError().databaseText());
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
    QString selectStatement = QString("SELECT %1 FROM %2 %3")
            .arg(getQueryColumnString(searchObject))
            .arg(m_tableName)
            .arg(whereClauseFind(searchObject));
    QSqlQuery query(db);
    bool result = query.prepare(selectStatement);
    if (! result) {
        SqlException exception(QString("Could not prepare statement !"), selectStatement, query.lastError().databaseText());
        throw exception;
    }
    foreach (QString key, searchObject.keys()) {
        if (hasValueForKey(searchObject, key)) {
            QString bindString = QString(':').append(key);
            query.bindValue(bindString, searchObject.value(key));
        }
    }
    result = query.exec();
    if (! result) {
        SqlException exception(QString("Could not execute query !"), selectStatement, query.lastError().databaseText());
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
    QSqlDatabase db = openDatabase();
    QSqlRecord record = db.record(tableName);
    if (record.isEmpty()) {
        db.close();
        throw SqlException(QString("could not read column names of database table !"), QString(), db.lastError().databaseText());
    }
    QStringList columnList;
    for (int index=0; index<record.count(); ++index) {
        columnList << record.fieldName(index);
    }
    db.close();

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
    QString queryString = QString("SELECT * FROM %1").arg(tableName);
    QSqlQuery query = db.exec(queryString);
    if (!query.isValid() && query.lastError().isValid()) {
        SqlException exception(QString("Could not read from database !"), queryString, query.lastError().databaseText());
        exception.setDatabaseError(query.lastError().driverText());
        exception.setSqlStatement(queryString);
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
    QSqlDatabase db = openDatabase();
    QSqlRecord record = db.record(m_tableName);
    if (record.isEmpty()) {
        throw SqlException(QString("Could not read table info !"), QString(), db.lastError().databaseText());
    }
    QHash<QString, QVariant::Type> dataTypeMap;
    for (int index=0; index<record.count(); ++index) {
        QSqlField field = record.field(index);
        QString columnName = field.name();
        QVariant::Type dataType = field.type();
        dataTypeMap.insert(columnName, dataType);
    }
    db.close();

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
 * Build a where clause to find values in database.
 * Returns a where condition string to search for
 * values.
 * @param searchObject
 * @return
 */
QString Persistence::whereClauseFind(const QVariantMap &searchObject) const
{
    QStringList keyList = searchObject.keys();
    QString whereClause("WHERE ");
    foreach (QString key, keyList) {
        QVariant value = searchObject.value(key);
        if (value.isValid()) {
            QString parameter = QString("%1=:%2 AND ").arg(key).arg(key);
            whereClause.append(parameter);
        }
    }
    if (whereClause.endsWith(QString("AND "))) {
        whereClause.remove(whereClause.length()-5, 5);
    }
    if (whereClause.endsWith(QString("WHERE "))) {
        return QString();
    }

    return whereClause;
}

/**
 * Tests if a map entry has a value or if it keeps
 * just an empty QVariant object.
 * @param searchObject      A map with column names as key and values to search for.
 * @param key               A key (column name) to proof.
 * @return                  True if entry has a valid value.
 */
bool Persistence::hasValueForKey(const QVariantMap &searchObject, const QString &key) const
{
    QVariant value = searchObject.value(key);

    return value.isValid();
}

/**
 * Takes a map with database column names as keys and creates a single
 * string with column names comma separated.
 * @param searchObject      A map with column names as key.
 * @return columnString     A single string with all column names comma separated.
 */
QString Persistence::getQueryColumnString(const QVariantMap &searchObject) const
{
    QStringList keyList = searchObject.keys();
    QString columns;
    foreach (QString key, keyList) {
        columns.append(key).append(',');
    }
    columns.remove(columns.length()-1, 1);

    return columns;
}

/**
 * Takes a string list with column names and creates a single
 * string with column names comma separated.
 * @param columnList        A list with column names.
 * @return columnString     A single string with all column names comma separated.
 */
QString Persistence::getQueryColumnString(const QStringList &columnList) const
{
    QString columnString;
    foreach (QString column, columnList) {
        columnString.append(column).append(',');
    }
    if (columnString.endsWith(QChar(','))) {
        columnString.remove(columnString.length()-1, 1);
    }

    return columnString;
}

/**
 * Build a set of tuples komma seperated.
 * Tuples consist of column name = bind string.
 * Bind string is column name with a leading colon.
 * @param differences   A map with all modified values.
 * @return tupleString  A string with UPDATE SET tuples.
 */
QString Persistence::updateTupleString(const QVariantMap &differences) const
{
    QString tupleString;
    QStringList keyList = differences.keys();
    foreach (QString key, keyList) {
        // columnName=:bindString,
        tupleString.append(key).append("=:").append(key).append(',');
    }
    tupleString.remove(tupleString.length()-1, 1);

    return tupleString;
}

/**
 * Creates a SQL command to insert data into a database table.
 * @param columnList    A list of column names.
 * @return insertSql    A string with a SQL insert command.
 */
QString Persistence::insertIntoSql(const QStringList &columnList) const
{
    QString insertSql = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(m_tableName)
                                                                .arg(getQueryColumnString(columnList))
                                                                .arg(placeholderString(columnList.count()));

    return insertSql;
}

/**
 * Creates a string with a given amount of placeholders.
 * For instance:    ?,?,?,?     (amount=4)
 * @param amount        The number of how many placeholders are needed.
 * @return placeholder  The string with the placeholders.
 */
QString Persistence::placeholderString(const int amount) const
{
    if (amount < 1) {
        return QString();
    }
    QString placeholder("?");
    for (int count=1; count<amount; ++count) {
        placeholder.append(QString(",?"));
    }

    return placeholder;
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

