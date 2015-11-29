#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "tablemodel.h"

class Persistence
{
public:
    Persistence();

    // Open and close persistence
    virtual bool open() const = 0;
    virtual void close() const = 0;
    virtual bool isOpen() const                     { return m_isOpen; }

    // Modify persistent objects.
    virtual bool persistAccountObject(const QVariantMap& account) const = 0;
    virtual bool deleteAccountObject(const QVariant& primaryKey) const = 0;
    virtual bool modifyAccountObject(const QVariantMap& modifiedObject) const = 0;

    // Read from persistence.
    virtual QVariantMap findAccount(const QVariant& primaryKey) const = 0;
    virtual QVariantMap findAccount(const QString& providerName, const QString& username) const = 0;
    virtual QList<QVariantMap> allPersistedAccounts() const = 0;

    // Model compability
    virtual QHash<int, QByteArray> getModelRoles() = 0;

protected:
    void setOpen(const bool isOpen)                 { m_isOpen = isOpen; }

private:
    bool m_isOpen;
};

#endif // PERSISTENCE_H
