#ifndef PERSISTENCE_H
#define PERSISTENCE_H

/* ------------------------------------------------------------------------------
 * Class Persistence
 *
 * Its an abstract class with most methods are pure virtual. This is just a
 * interface for a concret persistence class. There are a factory with creates
 * a instance of the concret class.
 * To use your own persistence you should subclass this interface and modify
 * the PersistenceFactory.
 * ------------------------------------------------------------------------------
 */

#include "modelconstants.h"
#include <QVariantMap>
#include <QAbstractListModel>

class Persistence
{
public:
    Persistence();
    virtual ~Persistence();

    // Open and close persistence
    virtual bool open() const = 0;
    virtual void close() const = 0;
    virtual bool isOpen() const;

    // Modify persistent objects.
    // Database connection must be made before call one of these methods.
    virtual bool persistAccountObject(const QVariantMap& account) const = 0;
    virtual bool deleteAccountObject(const QVariantMap& account) const = 0;
    virtual bool modifyAccountObject(const QVariantMap& modifiedObject) const = 0;
    virtual QVariantMap findAccount(const QVariantMap& searchObj) const = 0;

    // Read from persistence.
    // These methods open database connection by it self and close it afterwarts.
    virtual QList<QVariantMap> allPersistedAccounts() const = 0;

    // Model and Application compability
    // See description of standard implementation.
    virtual bool setModelHeader(QAbstractListModel* pModel) const;

protected:
    void setOpen(const bool isOpen)                 { m_isOpen = isOpen; }

private:
    bool m_isOpen;
};

#endif // PERSISTENCE_H
