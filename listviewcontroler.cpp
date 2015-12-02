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
void ListViewControler::persistModelModifications() const
{
    if (! m_pPersistence->open()) {
        qDebug() << "ListViewControler::persistModelModifications() --> m_pPersistence->open()";
        qDebug() << "Could not open persistence.";
        return;
    }
    int row = 0, lastRow = m_pModel->rowCount();
    while (row < lastRow) {
        QModelIndex index = m_pModel->index(row);
        TableModel::ModelRowState state = (TableModel::ModelRowState)m_pModel->data(index, TableModel::StateRole).toInt();
        switch (state) {
        case TableModel::New: {
            QVariantMap account = accountWithEditableRoles(index);
            if (! m_pPersistence->persistAccountObject(account)) {
                qDebug() << "ListViewControler::persistModelModifications() --> persistAccountObject(account)";
                qDebug() << "Could not persist Account object.";
            } else {
                // Read new Account from datafase.
                account = m_pPersistence->findAccount(account);
                // Update model. New Account got an id from database and last modify date.
                foreach (QString role, account.keys()) {
                    QVariant value = account.value(role);
                    m_pModel->setData(index, value, role);
                }
            }
            ++row;
            break;
        }
        case TableModel::Deleted: {
            QVariantMap account = m_pModel->getAccountObject(row);
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
        case TableModel::Modified: {
            QVariantMap account = m_pModel->getAccountObject(row);
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
    m_pModel->setHeaderContent(m_pPersistence->getModelHeader());
    QList<QVariantMap> persistentData = m_pPersistence->allPersistedAccounts();
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
QVariantMap ListViewControler::accountWithEditableRoles(const QModelIndex &index) const
{
    QVariantMap account;
    for (int column=0; column<m_pModel->columnCount(); ++column) {
        if (m_pModel->headerData(column, QString("editable")).toBool()) {
            QString role = m_pModel->headerData(column, QString("roleName")).toString();
            QVariant value = m_pModel->data(index, role);
            account.insert(role, value);
        }
    }

    return account;
}
