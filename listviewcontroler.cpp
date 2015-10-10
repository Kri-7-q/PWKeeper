#include "listviewcontroler.h"

ListViewControler::ListViewControler(QObject *parent) :
    Controller(parent)
{
    m_persistentData = m_database.readWholeTable(m_database.tableName());
    connect(this, SIGNAL(modelChanged()), this, SLOT(setModelContent()));
}

/**
 * Delete a row in the TableView model.
 * Marks a row as deleted in the model.
 * @param row
 */
void ListViewControler::deleteModelRow(const int row) const
{
    m_pModel->setData(m_pModel->index(row), QVariant(TableModel::Deleted), TableModel::StateRole);
}

/**
 * Slot
 * Is called when a model is set to the controler.
 */
void ListViewControler::setModelContent()
{
    m_pModel->resetContent(&m_persistentData);
}

