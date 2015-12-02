#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include "persistence.h"
#include <QSqlRecord>

class PostgreSql : public Persistence
{
public:
    PostgreSql();
    ~PostgreSql();

    // Persistence interface
public:
    bool open() const override;
    void close() const override;
    bool persistAccountObject(const QVariantMap &account) const override;
    bool deleteAccountObject(const QVariantMap &account) const override;
    bool modifyAccountObject(const QVariantMap &modifiedObject) const override;
    QVariantMap findAccount(const QVariantMap &searchObj) const override;
    QList<QVariantMap> allPersistedAccounts() const override;

private:
    QString m_databaseIdentifier;
    QString m_tableName;
    QSqlRecord m_record;

    // Private Methods
    void initialize();
    QVariantMap getAccountObject(const QSqlRecord& record) const;
    QSqlRecord recordWithIdentifier(const QVariantMap& account) const;
    QSqlRecord recordFromAccountObject(const QVariantMap& account) const;
    QVariantMap differenceToOrigin(const QVariantMap& origin, const QVariantMap& modified) const;
};

#endif // POSTGRESQL_H
