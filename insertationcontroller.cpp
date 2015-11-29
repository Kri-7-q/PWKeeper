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
void InsertationController::setModifiedData(const int row, const QVariantMap modifiedData)
{
    QModelIndex index = m_pModel->index(row);
    bool hasModifications = insertModifiedData(index, modifiedData);
    // Mark model row as modified if data was realy modified.
    TableModel::ModelRowState state = (TableModel::ModelRowState)m_pModel->data(index, TableModel::StateRole).toInt();
    if (hasModifications && state != TableModel::New) {
        m_pModel->setData(index, QVariant(TableModel::Modified), TableModel::StateRole);
    }
}

/**
 * Takes a QVariantMap from the AddDialog. Append a new row to the model.
 * Set data of QVariantMap to the model.
 * @param newData       A map with inserted data.
 * @param roleList      A list of model roles (Keys of the QVariantMap).
 */
int InsertationController::insertNewData(const QVariantMap newData)
{
    int row = m_pModel->appendEmptyRow();
    QModelIndex index = m_pModel->index(row);
    insertModifiedData(index, newData);
    m_pModel->setData(index, QVariant(TableModel::New), TableModel::StateRole);

    return row;
}

/**
 * Takes the Account data an generates a new password for this account.
 * It takes the password length value and character definition string
 * from the Account object.
 * @param account       An Account object with at least password length and definiton data.
 * @return              A new password.
 */
QString InsertationController::generatePassword(const QVariantMap account)
{
    QString roleName = m_pModel->modelRoleName(TableModel::DefinedCharacterRole);
    QString definition = account.value(roleName, QVariant(QString())).toString();
    roleName = m_pModel->modelRoleName(TableModel::LengthRole);
    int length = account.value(roleName, QVariant(0)).toInt();
    PwGenerator generator;
    QString password = generator.passwordFromDefinition(length, definition);
    if (generator.hasError()) {
        emit errorMessage(generator.errorMessage());
    }

    return password;
}

/**
 * Protected
 * Insert modified data into the model.
 * @param row               The models row where the data is to insert.
 * @param modifiedData      A QVariantMap with data of a model row.
 * @param roleList          The list with all used model role names.
 * @return hasModificatons  True if model was modified.
 */
bool InsertationController::insertModifiedData(const QModelIndex &index, const QVariantMap &modifiedData) const
{
    bool hasModifications = false;
    for(int column=0; column<m_pModel->columnCount(); ++column) {
        if (! m_pModel->headerData(column, "editable").toBool()) {
            continue;
        }
        QString role = m_pModel->headerData(column, "roleName").toString();
        QVariant value = modifiedData.value(role, QVariant());
        QVariant original = m_pModel->data(index, role);
        QVariant::Type dataType = m_pModel->headerData(column, "dataType").type();
        if (value.convert(dataType)) {
            if (value != original) {
                m_pModel->setData(index, value, role);
                hasModifications = true;
            }
        }
    }

    return hasModifications;
}
