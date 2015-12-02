#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "tablemodel.h"

class Persistence
{
public:
    Persistence();
    virtual ~Persistence();

    // Below ModelRoles should be used in 'getModelHeader()'
    // PrimaryKey, Provider, Username, Password, PasswordLength, DefinedCharacter MUST be present.
    // The others are optional.
    enum ModelRole { PrimaryKeyRole = Qt::UserRole, ProviderRole, UsernameRole, PasswordRole, PasswordLengthRole,
                     DefinedCharacterRole, QuestionRole, AnswerRole, LastModifyRole};

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
    // These methods open database connection by their selfs and close it afterwarts.
    virtual QList<QVariantMap> allPersistedAccounts() const = 0;

    // Model and Application compability
    virtual QList<QVariantMap> getModelHeader() const;

protected:
    void setOpen(const bool isOpen)                 { m_isOpen = isOpen; }

private:
    bool m_isOpen;
};

#endif // PERSISTENCE_H
