#include "listviewcontroler.h"

ListViewControler::ListViewControler(QObject *parent) :
    QObject(parent),
    m_pModel(NULL)
{
    m_persistentData = m_database.readWholeTable(m_database.tableName());
}

// Getter
TableModel *ListViewControler::model() const
{
    return m_pModel;
}

// Setter
void ListViewControler::setModel(TableModel *pModel)
{
    if (m_pModel != pModel) {
        m_pModel = pModel;
        m_pModel->resetContent(&m_persistentData);
        emit modelChanged();
    }
}

/**
 * Delete a row in the TableView model.
 * Marks a row as deleted in the model.
 * @param row
 */
void ListViewControler::deleteModelRow(const int row) const
{
    m_pModel->setData(m_pModel->index(row), QVariant(TableModel::Deleted), QString("state"));
}

