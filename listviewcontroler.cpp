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
 * If a row has model row state New than the row is deleted immediately.
 * @param row
 */
void ListViewControler::deleteModelRow(const int row) const
{
    QModelIndex index = m_pModel->index(row);
    TableModel::ModelRowState state = (TableModel::ModelRowState)m_pModel->data(index, TableModel::StateRole).toInt();
    if (state == TableModel::New) {
        m_pModel->removeRow(row);
    } else {
        m_pModel->setData(index, QVariant(TableModel::Deleted), TableModel::StateRole);
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
    m_pModel->resetContent(&m_persistentData);
    QHash<QString, QVariant::Type> dataTypeMap = m_database.getTablesDataTypes();
    m_pModel->setDataTypeMap(dataTypeMap);
}

