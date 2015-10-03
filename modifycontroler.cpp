#include "modifycontroler.h"

ModifyControler::ModifyControler(QObject *parent) :
    QObject(parent),
    m_pModel(NULL)
{

}

/**
 * Get controlers model.
 * @return Pointer to the controler model.
 */
TableModel *ModifyControler::model() const
{
    return m_pModel;
}

/**
 * Set a model to the controler.
 * @param pModel    The model to set.
 */
void ModifyControler::setModel(TableModel *pModel)
{
    if (m_pModel != pModel) {
        m_pModel = pModel;
        emit modelChanged();
    }
}

/**
 * Returns all data of a model row.
 * @param row       The row of requested data.
 * @return          A QVariantMap obect containing the rows data.
 */
QVariantMap ModifyControler::modelRowEntry(const int row) const
{
    return m_pModel->getRow(row);
}

/**
 * Takes a QVariantMap with modified data of a model row entry.
 * All of this data MUST be convertable to the type of this in
 * the model.The come each as a QString from TextField controls.
 * If any of this data is different to this in the model then
 * the new data is stored in model.
 * @param row               The row with was modified.
 * @param modifiedData      The modified data.
 * @param roleList          The list of editable roles.
 *                          The Id and lastmodify date are not
 *                          editable.
 */
void ModifyControler::setModifiedData(const int row, const QVariantMap modifiedData, const QStringList roleList)
{
    bool hasModifications = false;
    QModelIndex index = m_pModel->index(row);
    foreach (QString role, roleList) {
        QVariant value = modifiedData.value(role, QVariant());
        QVariant original = m_pModel->data(index, role);
        if (value.convert(original.type())) {
            if (value != original) {
                m_pModel->setData(index, value, role);
                hasModifications = true;
            }
        }
    }
    // Mark model row as modified if data was realy modified.
    if (hasModifications) {
        m_pModel->setData(index, QVariant(TableModel::Modified), "state");
    }
}

/**
 * Takes the Account data an generates a new password for this account.
 * It takes the password length value and character definition string
 * from the Account object.
 * @param account       An Account object with at least password length and definiton data.
 * @return              A new password.
 */
QString ModifyControler::generatePassword(const QVariantMap account) const
{
    QString roleName = m_pModel->modelRoleName(TableModel::DefinedCharacterRole);
    QString definition = account.value(roleName, QVariant(QString())).toString();
    roleName = m_pModel->modelRoleName(TableModel::LengthRole);
    int length = account.value(roleName, QVariant(0)).toInt();
    PwGenerator generator;

    return generator.passwordFromDefinition(length, definition);
}

