#ifndef PSQLDATABASE_H
#define PSQLDATABASE_H

#include "Exception/sqlexception.h"
#include <QStringList>
#include <QVariantMap>
#include <QSqlDatabase>
#include <QSqlRecord>

class PSqlDatabase
{
public:
    PSqlDatabase();

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
    QList<QVariantMap> readWholeTable() const;
    QHash<QString, QVariant::Type> getTablesDataTypes() const;

    // Static
    static QHash<int, QByteArray> getTableModelRoles();

private:
    QString m_primaryKey;
    QStringList m_uniqueKey;
    QString m_tableName;
    QSqlRecord m_record;

    // Methods
    QSqlRecord recordFromVariantMap(const QVariantMap& searchObject) const;
    QSqlRecord recordFieldsWithValuesFromRecord(const QSqlRecord& sourceRecord) const;
    QList<QVariantMap> getAccountList(QSqlQuery &query) const;
    QVariantMap getAccountObject(const QSqlQuery& query, const QSqlRecord &record) const;
    void initializeDatabase(QSqlDatabase &db) const;
};

#endif // PSQLDATABASE_H
