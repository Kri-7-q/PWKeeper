#include "persistence.h"

Persistence::Persistence() :
    m_primaryKey(QString("id")),
    m_uniqueKey(QStringList() << QString("provider") << QString("username")),
    m_tableName("account")
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "local");
    db.setDatabaseName(QString("pwmanager"));
    db.setHostName(QString("localhost"));
    db.setPassword(QString("kri-7-q"));
    db.setUserName(QString("christian"));
    db.setPort(3306);
}

/**
 * @brief Persistence::findAccounts
 * @param searchObject
 * @return
 */
QList<QVariantMap> Persistence::findAccounts(const QVariantMap &searchObject) const
{
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    if (! db.open()) {
        throw SqlException(QString("Could not open database !"));
    }
    QString selectStatement = QString("SELECT %1 FROM %2 %3")
            .arg(queryColumns(searchObject))
            .arg(m_tableName)
            .arg(findWhereClause(searchObject));
    QSqlQuery query(db);
    bool result = query.prepare(selectStatement);
    if (! result) {
        SqlException exception("Could not prepare statement !");
        exception.setSqlStatement(selectStatement);
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
        SqlException exception(QString("Could not execute query !"));
        exception.setSqlStatement(selectStatement);
        throw exception;
    }
    QList<QVariantMap> accountList = getAccountList(query);
    db.close();

    return accountList;
}

/**
 * Get all column names of a database table.
 * @param tableName
 * @return              A list of all column names.
 */
QStringList Persistence::getColumnNames(const QString tableName) const
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (!db.open()) {
        throw SqlException(QString("Could not open database !"));
    }
    QSqlRecord record = db.record(tableName);
    if (record.isEmpty()) {
        throw SqlException(QString("could not read column names of database table !"));
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
 * @param tableName     The name of the database table.
 * @return              A list with all table content.
 */
QList<QVariantMap> Persistence::readWholeTable(const QString tableName) const
{
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    if (! db.open()) {
        throw SqlException(QString("Could not open database !"));
    }
    QString queryString = QString("SELECT * FROM %1").arg(tableName);
    QSqlQuery query = db.exec(queryString);
    if (!query.isValid() && query.lastError().isValid()) {
        SqlException exception(QString("Could not read from database !"));
        exception.setDatabaseError(query.lastError().driverText());
        exception.setSqlStatement(queryString);
        throw exception;
    }
    QList<QVariantMap> accountList = getAccountList(query);
    db.close();

    return accountList;
}

/**
 * Build a where clause to find values in database.
 * @param searchObject
 * @return
 */
QString Persistence::findWhereClause(const QVariantMap &searchObject) const
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
 * @brief Persistence::hasValueForKey
 * @param searchObject
 * @param key
 * @return
 */
bool Persistence::hasValueForKey(const QVariantMap &searchObject, const QString &key) const
{
    QVariant value = searchObject.value(key);

    return value.isValid();
}

/**
 * @brief Persistence::queryColumns
 * @param searchObject
 * @return
 */
QString Persistence::queryColumns(const QVariantMap &searchObject) const
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
QString Persistence::queryColumns(const QStringList &columnList) const
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
 * @brief Persistence::getAccountList
 * @param result
 * @return
 */
QList<QVariantMap> Persistence::getAccountList(QSqlQuery &query) const
{
    QList<QVariantMap> list;
    QStringList columnList;
    for (int i=0; i<query.record().count(); ++i)
        columnList << query.record().fieldName(i);
    while (query.next()) {
        QVariantMap account;
        for (int i=0; i<columnList.size(); ++i) {
            account.insert(columnList.at(i), query.value(i));
        }
        list << account;
    }

    return list;
}

