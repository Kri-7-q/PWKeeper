#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_pModel(NULL)
{

}

/**
 * Property
 * Set model pointer to controller.
 * @param pModel    A TableModel pointer.
 */
void Controller::setModel(TableModel *pModel)
{
    if (m_pModel != pModel) {
        m_pModel = pModel;
        emit modelChanged();
    }
}

/**
 * Property
 * Get model pointer.
 * @return A TableModel pointer.
 */
TableModel *Controller::model() const
{
    return m_pModel;
}

/**
 * Slot
 * Get the whole row entry of a model.
 * @param row       The nummer of row which is requested.
 * @return          A QVariantMap with a model entry.
 */
QVariantMap Controller::modelRowEntry(const int row) const
{
    return m_pModel->getRow(row);
}

