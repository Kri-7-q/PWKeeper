#include "listviewcontroler.h"
#include <QDateTime>

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
void ListViewControler::deleteModelRow(const int row) const
{
    QModelIndex index = m_pModel->index(row);
    ModelRowState state = (ModelRowState)m_pModel->data(index, StateRole).toInt();
    if (state == New) {
        m_pModel->removeRow(row, QModelIndex());
    } else {
        m_pModel->setData(index, QVariant(Deleted), StateRole);
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
        ModelRowState state = (ModelRowState)modelRowState(row).toInt();
        switch (state) {
        case New: {
            QVariantMap account = accountWithEditableRoles(row);
            if (! m_pPersistence->persistAccountObject(account)) {
                qDebug() << "ListViewControler::persistModelModifications() --> persistAccountObject(account)";
                qDebug() << "Could not persist Account object.";
            } else {
                // Read new Account from datafase.
                account = m_pPersistence->findAccount(account);
                // Update model. New Account got an id from database and last modify date.
                removeAccountObject(row, account);
            }
            ++row;
            break;
        }
        case Deleted: {
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
        case Modified: {
            QVariantMap account = accountObjectFromModel(row);
            if (! m_pPersistence->modifyAccountObject(account)) {
                qDebug() << "ListViewControler::persistModelModifications() --> modifyAccountObject(account)";
                qDebug() << "Could not modify Account object.";
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
}

/**
 * Get the current state of a model row (Account object).
 * The states are: Origin, New, Modified, Deleted
 * @param row       The row number of Account object.
 * @return          The current state of the object.
 */
QVariant ListViewControler::modelRowState(const int row) const
{
    QModelIndex index = m_pModel->index(row);
    QVariant state = m_pModel->data(index, StateRole);
    if (! state.isValid()) {
        return QVariant(Origin);
    }

    return state;
}

/**
 * Get a list of visible TableView columns.
 * Column objects contain header name and role name to create
 * a TableViewColumn object.
 * @return      A list of visible columns.
 */
QList<QVariantMap> ListViewControler::getVisibleColumns() const
{
    QList<QVariantMap> columnList;
    for (int column=0; column<m_pModel->columnCount(); ++column) {
        bool visible = m_pModel->headerData(column, Qt::Horizontal, ColumnVisibleRole).toBool();
        if (visible) {
            QVariantMap columnObj;
            QVariant headerName = m_pModel->headerData(column, Qt::Horizontal, HeaderNameRole);
            QVariant roleName = m_pModel->headerData(column, Qt::Horizontal, DataRoleNameRole);
            columnObj.insert("headerName", headerName);
            columnObj.insert("roleName", roleName);
            columnList << columnObj;
        }
    }

    return columnList;
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
 * Removes an Account object in the model.
 * @param row           The model row where account should be removed.
 * @param account       The Account object to be inserted instead of another one.
 */
void ListViewControler::removeAccountObject(const int row, const QVariantMap &account)
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
