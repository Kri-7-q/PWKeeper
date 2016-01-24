#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_pModel(NULL)
{

}

/**
 * Property
 * Set model pointer to controller.
 * @param pModel    A model pointer.
 */
void Controller::setModel(AbstractTableViewModel *pModel)
{
    if (m_pModel != pModel) {
        m_pModel = pModel;
        emit modelChanged();
    }
}

/**
 * Property
 * Get model pointer.
 * @return A model pointer.
 */
AbstractTableViewModel *Controller::model() const
{
    return m_pModel;
}

/**
 * Get data from model by column (section) and row number.
 * @param section       The models column.
 * @param row           Row number of model entry.
 * @return              The value from the model.
 */
QVariant Controller::modelData(const int section, const int row) const
{
    int role = m_pModel->headerData(section, Qt::Horizontal, DataRoleIdRole).toInt();
    QModelIndex index = m_pModel->index(row);

    return m_pModel->data(index, role);
}

/**
 * Get the header text value of a model column. This text is
 * normaly be shown in table view header.
 * @param column    The column to query for its header text.
 * @return          A QVariant with the header text.
 */
QVariant Controller::modelHeaderText(const int column) const
{
    return m_pModel->headerData(column, Qt::Horizontal, HeaderNameRole);
}

/**
 * SLOT
 * Get a standard value to a column of the model header.
 * @param column        The column to query for if standard value.
 * @return              The standard value to the column.
 */
QVariant Controller::standardText(const int column) const
{
    return m_pModel->headerData(column, Qt::Horizontal, StandardValueRole);
}
