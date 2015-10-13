#include "addcontroler.h"

AddControler::AddControler(QObject *parent) :
    InsertationController(parent)
{

}


/**
 * Append an empty new row to the model.
 * Should be called before open AddDialog.
 * It ensures that the model can provide data for the text field in dialog.
 * @return
 */
int AddControler::appendEmptyModelRow() const
{
    QVariantMap emptyRow;
    emptyRow.insert(m_pModel->modelRoleName(TableModel::LengthRole), QVariant(12));
    emptyRow.insert(m_pModel->modelRoleName(TableModel::DefinedCharacterRole), QVariant(QString("*[A-Z]*[a-z]*[0-9]")));

    return m_pModel->appendEmptyRow(emptyRow);
}
