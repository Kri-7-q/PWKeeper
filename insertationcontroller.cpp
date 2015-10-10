#include "insertationcontroller.h"

InsertationController::InsertationController(QObject *parent) :
    Controller(parent)
{

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
void InsertationController::setModifiedData(const int row, const QVariantMap modifiedData, const QStringList roleList)
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
        m_pModel->setData(index, QVariant(TableModel::Modified), TableModel::StateRole);
    }
}

/**
 * Takes a QVariantMap from the AddDialog. Append a new row to the model.
 * Set data of QVariantMap to the model.
 * @param newData       A map with inserted data.
 * @param roleList      A list of model roles (Keys of the QVariantMap).
 */
void InsertationController::insertNewData(const QVariantMap newData, const QStringList roleList)
{
    int row = m_pModel->appendRow();
    if (row < 0) {
        qDebug() << "Could not append new model row !";
        return;
    }
    QModelIndex index = m_pModel->index(row);
    foreach (QString role, roleList) {
        QVariant value = newData.value(role);
        m_pModel->setData(index, value, role);
    }
    m_pModel->setData(index, QVariant(TableModel::New), TableModel::StateRole);
}

/**
 * Takes the Account data an generates a new password for this account.
 * It takes the password length value and character definition string
 * from the Account object.
 * @param account       An Account object with at least password length and definiton data.
 * @return              A new password.
 */
QString InsertationController::generatePassword(const QVariantMap account) const
{
    QString roleName = m_pModel->modelRoleName(TableModel::DefinedCharacterRole);
    QString definition = account.value(roleName, QVariant(QString())).toString();
    roleName = m_pModel->modelRoleName(TableModel::LengthRole);
    int length = account.value(roleName, QVariant(0)).toInt();
    PwGenerator generator;

    return generator.passwordFromDefinition(length, definition);
}
