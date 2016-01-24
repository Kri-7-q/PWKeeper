#include "listviewcontroler.h"
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

ListViewControler::ListViewControler(QObject *parent) :
    Controller(parent)
{
    m_pPersistence = PersistenceFactory::createPersistenceObj();
    connect(this, SIGNAL(modelChanged()), this, SLOT(setModelContent()));
}

/**
 * Destructor
 */
ListViewControler::~ListViewControler()
{
    delete m_pPersistence;
}

/**
 * Delete a row in the TableView model.
 * Marks a row as deleted in the model but keep it until deleted from persistence.
 * If a row has model row state New than the row is deleted immediately.
 * @param row
 */
void ListViewControler::deleteModelRow(const int row)
{
    QModelIndex index = m_pModel->index(row);
    AbstractTableViewModel::DataState state = (AbstractTableViewModel::DataState)m_pModel->data(index, StateRole).toInt();
    if (state == AbstractTableViewModel::New) {
        m_pModel->removeRow(row, QModelIndex());
    } else {
        m_pModel->setModelRowState(row, AbstractTableViewModel::Deleted);
    }
}

/**
 * Store all modifications made on the model.
 * Store new model rows and modified rows into
 * database and delete Accounts from database.
 */
void ListViewControler::persistModelModifications()
{
    if (! m_pPersistence->open()) {
        qDebug() << "ListViewControler::persistModelModifications() --> m_pPersistence->open()";
        qDebug() << "Could not open persistence.";
        return;
    }
    int row = 0, lastRow = m_pModel->rowCount();
    while (row < lastRow) {
        AbstractTableViewModel::DataState state = m_pModel->modelRowState(row);
        switch (state) {
        case AbstractTableViewModel::New: {
            QVariantMap account = accountWithEditableRoles(row);
            if (! m_pPersistence->persistAccountObject(account)) {
                qDebug() << "ListViewControler::persistModelModifications() --> persistAccountObject(account)";
                qDebug() << "Could not persist Account object.";
            } else {
                // Read new Account from datafase.
                account = m_pPersistence->findAccount(account);
                // Update model. New Account got an id from database and last modify date.
                replaceAccountObject(row, account);
                emit selectTableViewRow(row);
                m_pModel->setModelRowState(row, AbstractTableViewModel::Origin);
            }
            ++row;
            break;
        }
        case AbstractTableViewModel::Deleted: {
            QVariantMap account = accountObjectFromModel(row);
            if (! m_pPersistence->deleteAccountObject(account)) {
                qDebug() << "ListViewControler::persistModelModifications() --> deleteAccountObject(account)";
                qDebug() << "Could not delete Account object.";
                ++row;
            } else {
                m_pModel->removeRow(row);
                lastRow = m_pModel->rowCount();
            }
            break;
        }
        case AbstractTableViewModel::Modified: {
            QVariantMap account = accountObjectFromModel(row);
            if (! m_pPersistence->modifyAccountObject(account)) {
                qDebug() << "ListViewControler::persistModelModifications() --> modifyAccountObject(account)";
                qDebug() << "Could not modify Account object.";
            } else {
                emit selectTableViewRow(row);
                m_pModel->setModelRowState(row, AbstractTableViewModel::Origin);
            }
            ++row;
            break;
        }
        default:
            ++row;
            break;
        }
    }
    m_pPersistence->close();
    emit selectTableViewRow(-1);
}

/**
 * Get the number of columns in the model.
 * @return
 */
int ListViewControler::columnCount() const
{
    return m_pModel->columnCount();
}

/**
 * Get information about a model column.
 * Return if a column should be visible in TableView or not.
 * @param column    The column to query for information.
 * @return          A QVariant object with a bool value.
 */
QVariant ListViewControler::isColumnVisible(const int column) const
{
    return m_pModel->headerData(column, Qt::Horizontal, ColumnVisibleRole);
}

/**
 * Get the role name of a model column. (QVariantMap key)
 * The role names are part of the model header data.
 * @param column    The column to query for the role name.
 * @return          A QVariant with the role name.
 */
QVariant ListViewControler::roleNameOfColumn(const int column) const
{
    return m_pModel->headerData(column, Qt::Horizontal, DataRoleNameRole);
}

/**
 * Private Slot
 * Is called when a model is set to the controler.
 * Signal is connected to this slot in constructor.
 * All content of database is set to the model.
 * Model header data are set from Persistence.
 * Signal 'headerDataChanged()' is emitted. This
 * forces the TableView to create its columns.
 */
void ListViewControler::setModelContent()
{
    m_pPersistence->setModelHeader(m_pModel);
    m_pModel->resetModelRoles();
    QList<QVariantMap> persistentData = m_pPersistence->allPersistedAccounts();
    m_pModel->resetContent(&persistentData);
    emit modelContentAvailable();
}

/**
 * Reads all editable values of an account from the model.
 * Values are stored in a QVariantMap with model role names
 * (database column names) as keys.
 * If a value is editable or not can be queried from models
 * header data.
 * @param index             Model index of the Account object.
 * @return account          A QVariantMap map with the Account object.
 */
QVariantMap ListViewControler::accountWithEditableRoles(const int row) const
{
    QModelIndex index = m_pModel->index(row);
    QVariantMap account;
    for (int column=0; column<m_pModel->columnCount(); ++column) {
        if (m_pModel->headerData(column, Qt::Horizontal, EditableRole).toBool()) {
            int roleId = m_pModel->headerData(column, Qt::Horizontal, DataRoleIdRole).toInt();
            QString role = m_pModel->headerData(column, Qt::Horizontal, DataRoleNameRole).toString();
            QVariant value = m_pModel->data(index, roleId);
            if (value.isValid()) {
                account.insert(role, value);
            }
        }
    }

    return account;
}

/**
 * Public
 * Get an Account object from the model by its row number.
 * @param row       The number of row in model.
 * @return account  An Account object.
 */
QVariantMap ListViewControler::accountObjectFromModel(const int row) const
{
    QModelIndex index = m_pModel->index(row);
    QVariantMap account;
    for (int column=0; column<m_pModel->columnCount(); ++column) {
        int roleId = m_pModel->headerData(column, Qt::Horizontal, DataRoleIdRole).toInt();
        QString roleName = m_pModel->headerData(column, Qt::Horizontal, DataRoleNameRole).toString();
        QVariant value = m_pModel->data(index, roleId);
        if (value.isValid()) {
            account.insert(roleName, value);
        }
    }

    return account;
}

/**
 * Public
 * Replaces an Account object in the model.
 * @param row           The model row where account should be replaced.
 * @param account       The Account object which should replace the one stored there.
 */
void ListViewControler::replaceAccountObject(const int row, const QVariantMap &account)
{
    QModelIndex index = m_pModel->index(row);
    for (int column=0; column<m_pModel->columnCount(); ++column) {
        int role = m_pModel->headerData(column, Qt::Horizontal, DataRoleIdRole).toInt();
        QString roleName = m_pModel->headerData(column, Qt::Horizontal, DataRoleNameRole).toString();
        QVariant defaultValue = m_pModel->headerData(column, Qt::Horizontal, DataTypeRole);
        QVariant value = account.value(roleName, defaultValue);
        m_pModel->setData(index, value, role);
    }
}
