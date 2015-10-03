#include "showcontroler.h"

ShowControler::ShowControler(QObject *parent) : QObject(parent)
{

}

// Getter
TableModel *ShowControler::model() const
{
    return m_pModel;
}

// Setter
void ShowControler::setModel(TableModel *pModel)
{
    if (m_pModel != pModel) {
        m_pModel = pModel;
        emit modelChanged();
    }
}

/**
 * Get data of a whole row from model.
 * @param row       The row in the model.
 * @return          A QVariantMap object with the entry data.
 */
QVariantMap ShowControler::modelRowEntry(const int row) const
{
    return m_pModel->getRow(row);
}

