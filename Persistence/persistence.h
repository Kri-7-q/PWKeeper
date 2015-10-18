#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "Exception/sqlexception.h"
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariantMap>

class Persistence
{
public:
    Persistence();

    // Gettter and Setter
    QString tableName() const                       { return m_tableName; }

    // Open and close database
    QSqlDatabase openDatabase() const;
    void closeDatabase(QSqlDatabase& db) const;

    // Modify database rows.
    bool persistAccountObject(const QVariantMap& account, const QSqlDatabase& db) const;
    bool deleteAccountObject(const int objectId, const QSqlDatabase& db) const;
    bool modifyAccountObject(const int id, const QVariantMap& modifications, const QSqlDatabase& db) const;

    // Public methods
    QVariantMap findAccount(const int id, const QSqlDatabase& db) const;
    QVariantMap findAccount(const QString& providerName, const QString& username, const QSqlDatabase &db) const;
    QList<QVariantMap> findAccounts(const QVariantMap &searchObject) const;
    QStringList getColumnNames(const QString tableName) const;
    QList<QVariantMap> readWholeTable(const QString tableName) const;
    QHash<QString, QVariant::Type> getTablesDataTypes() const;

    // Static
    static QHash<int, QByteArray> getTableModelRoles();

private:
    QString m_primaryKey;
    QStringList m_uniqueKey;
    QString m_tableName;

    // Methods
    QString whereClauseFind(const QVariantMap &searchObject) const;
    bool hasValueForKey(const QVariantMap &searchObject, const QString &key) const;
    QString getQueryColumnString(const QVariantMap &searchObject) const;
    QString getQueryColumnString(const QStringList &columnList) const;
    QString updateTupleString(const QVariantMap& differences) const;
    QString insertIntoSql(const QStringList& columnList) const;
    QString placeholderString(const int amount) const;
    QList<QVariantMap> getAccountList(QSqlQuery &query) const;
    QVariantMap getAccountObject(const QSqlQuery& query, const QSqlRecord &record) const;
    void initializeDatabase(QSqlDatabase &db) const;
};

#endif // PERSISTENCE_H
