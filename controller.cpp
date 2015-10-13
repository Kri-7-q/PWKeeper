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
