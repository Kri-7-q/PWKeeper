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
