#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "sqlexception.h"
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

    // Public methods
    QList<QVariantMap> findAccounts(const QVariantMap &searchObject) const;
    QStringList getColumnNames(const QString tableName) const;
    QList<QVariantMap> readWholeTable(const QString tableName) const;

private:
    QString m_primaryKey;
    QStringList m_uniqueKey;
    QString m_tableName;

    // Methods
    QString findWhereClause(const QVariantMap &searchObject) const;
    bool hasValueForKey(const QVariantMap &searchObject, const QString &key) const;
    QString getQueryColumnString(const QVariantMap &searchObject) const;
    QString getQueryColumnString(const QStringList &columnList) const;
    QList<QVariantMap> getAccountList(QSqlQuery &query) const;
};

#endif // PERSISTENCE_H
