#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include "persistence.h"
#include <QSqlRecord>

class PostgreSql : public Persistence
{
public:
    PostgreSql();

    // Persistence interface
public:
    bool open() const override;
    void close() const override;
    bool persistAccountObject(const QVariantMap &account) const override;
    bool deleteAccountObject(const QVariant &primaryKey) const override;
    bool modifyAccountObject(const QVariantMap &modifiedObject) const override;
    QVariantMap findAccount(const QVariant &primaryKey) const override;
    QVariantMap findAccount(const QString &providerName, const QString &username) const override;
    QList<QVariantMap> allPersistedAccounts() const override;

private:
    QString m_databaseIdentifier;
    QString m_tableName;

    // Private Methods
    void initialize();
    QVariantMap getAccountObject(const QSqlRecord& record) const;
    QSqlRecord getRecordFromAccountObject(const QVariantMap& account) const;
    QVariantMap getModifications(const QVariantMap& origin, const QVariantMap& modified) const;
};

#endif // POSTGRESQL_H
