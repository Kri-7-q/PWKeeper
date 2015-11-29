#include "listviewcontroler.h"
#include <QDateTime>

ListViewControler::ListViewControler(QObject *parent) :
    Controller(parent)
{
    connect(this, SIGNAL(modelChanged()), this, SLOT(setModelContent()));
}

/**
 * Delete a row in the TableView model.
 * Marks a row as deleted in the model.
 * If a row has model row state New than the row is deleted immediately.
 * @param row
 */
void ListViewControler::deleteModelRow(const int row) const
{
    QModelIndex index = m_pModel->index(row);
    TableModel::ModelRowState state = (TableModel::ModelRowState)m_pModel->data(index, TableModel::StateRole).toInt();
    if (state == TableModel::New) {
        m_pModel->removeRow(row, QModelIndex());
    } else {
        m_pModel->setData(index, QVariant(TableModel::Deleted), TableModel::StateRole);
    }
}

/**
 * Store all modifications made on the model.
 * Store new model rows and modified rows into
 * database and delete Accounts from database.
 */
void ListViewControler::persistModelModifications(const QStringList editableRoles) const
{
    QSqlDatabase db;
    try {
    db = m_database.openDatabase();
    int row = 0, lastRow = m_pModel->rowCount();
    while (row < lastRow) {
        QModelIndex index = m_pModel->index(row);
        TableModel::ModelRowState state = (TableModel::ModelRowState)m_pModel->data(index, TableModel::StateRole).toInt();
        switch (state) {
        case TableModel::New: {
            QVariantMap account = newAccountFromModel(index, editableRoles);
            m_database.persistAccountObject(account, db);
            // Read new Account from datafase.
            QString provider = m_pModel->data(index, TableModel::ProviderRole).toString();
            QString username = m_pModel->data(index, TableModel::UsernameRole).toString();
            account = m_database.findAccount(provider, username, db);
            // Update model. New Account got an id from database and last modify date.
            QString role = m_pModel->modelRoleName(TableModel::IdRole);
            m_pModel->setData(index, account.value(role), role);
            role = m_pModel->modelRoleName(TableModel::LastModifyRole);
            m_pModel->setData(index, account.value(role), role);
            m_pModel->setData(index, QVariant(TableModel::Origin), TableModel::StateRole);
            ++row;
            break;
        }
        case TableModel::Deleted: {
            int objectId = m_pModel->data(index, TableModel::IdRole).toInt();
            m_database.deleteAccountObject(objectId, db);
            m_pModel->removeRow(row);
            lastRow = m_pModel->rowCount();
            break;
        }
        case TableModel::Modified: {
            int id = m_pModel->data(index, TableModel::IdRole).toInt();
            QVariantMap originAccount = m_database.findAccount(id, db);
            QVariantMap differences = modifications(index, originAccount);
            m_database.modifyAccountObject(id, differences, db);
            m_pModel->setData(index, QVariant(TableModel::Origin), TableModel::StateRole);
            ++row;
            break;
        }
        default:
            ++row;
            break;
        }
    }
    db.close();
    } catch (SqlException exception) {
        qDebug() << exception.errorText();
        qDebug() << exception.databaseError();
        qDebug() << exception.sqlStatement();
        db.close();
    }
}

/**
 * Private
 * Slot
 * Is called when a model is set to the controler.
 * Signal is connected to this slot in constructor.
 * All content of database is set to the model.
 * Data types to the model roles are read from database.
 * (Role names are equal to database column names.)
 */
void ListViewControler::setModelContent()
{
    QList<QVariantMap> persistentData = m_database.readWholeTable();
    m_pModel->resetContent(&persistentData);
}

/**
 * Reads all editable values of a new account in the model.
 * Values are stored in a QVariantMap with model role names
 * (database column names) as keys.
 * Adds the current date and time to LastModifyRole.
 * @param index             Model index of the new entry.
 * @param editableRoles     A list of all editable model roles.
 * @return account          A QVariantMap map with the new Account object.
 */
QVariantMap ListViewControler::newAccountFromModel(const QModelIndex &index, const QStringList &editableRoles) const
{
    QVariantMap account;
    foreach (QString role, editableRoles) {
        QVariant value = m_pModel->data(index, role);
        account.insert(role, value);
    }
    QString role = m_pModel->modelRoleName(TableModel::LastModifyRole);
    QVariant value(QDateTime::currentDateTime());
    account.insert(role, value);

    return account;
}

/**
 * Takes the origin Account object and a model index. The origin Account
 * is compared to the Account object in the model. All differences are
 * returned in a QVariantMap object.
 * @param index     The index of the modified Account object in the model.
 * @param origin    The origin Account object before modification.
 * @return          The differences between origin and modified Account.
 */
QVariantMap ListViewControler::modifications(const QModelIndex &index, const QVariantMap &origin) const
{
    QVariantMap differeces;
    QStringList roleList = origin.keys();
    foreach (QString roleName, roleList) {
        QVariant originValue = origin.value(roleName);
        QVariant modifiedValue = m_pModel->data(index, roleName);
        if (modifiedValue != originValue) {
            differeces.insert(roleName, modifiedValue);
        }
    }

    return differeces;
}
