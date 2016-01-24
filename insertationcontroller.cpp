#include "insertationcontroller.h"
#include "Persistence/persistence.h"

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
    AbstractTableViewModel::DataState state = m_pModel->modelRowState(row);
    if (hasModifications && state != AbstractTableViewModel::New) {
        m_pModel->setModelRowState(row, AbstractTableViewModel::Modified);
    }
}

/**
 * Takes a QVariantMap from the AddDialog. Append a new row to the model.
 * Set data of QVariantMap to the model.
 * @param newData       A map with inserted data.
 * @param roleList      A list of model roles (Keys of the QVariantMap).
 */
void InsertationController::insertNewData(const QVariantMap newData)
{
    int row = m_pModel->rowCount();
    m_pModel->insertRow(row);
    QModelIndex index = m_pModel->index(row);
    insertModifiedData(index, newData);
    m_pModel->setModelRowState(row, AbstractTableViewModel::New);
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
    QString roleName = m_pModel->modelRoleName(DefinedCharacterRole);
    QString definition = account.value(roleName, QVariant(QString())).toString();
    roleName = m_pModel->modelRoleName(PasswordLengthRole);
    int length = account.value(roleName, QVariant(0)).toInt();
    PwGenerator generator;
    QString password = generator.passwordFromDefinition(length, definition);
    if (generator.hasError()) {
        emit errorMessage(generator.errorMessage());
    }

    return password;
}

/**
 * Get the placeholder string from model header.
 * @param section       The models column.
 * @return              The placeholder text as a QVariant.
 */
QVariant InsertationController::modelHeaderPlaceHolder(const int section) const
{
    return m_pModel->headerData(section, Qt::Horizontal, PlaceHolderRole);
}

/**
 * Get the role name for data of a model column.
 * @param section       The models column.
 * @return              The role name.
 */
QVariant InsertationController::modelHeaderRoleName(const int section) const
{
    return m_pModel->headerData(section, Qt::Horizontal, DataRoleNameRole);
}

/**
 * Get information about a model column. If data is editable.
 * @param section       The models column.
 * @return              True if column data is editable.
 */
QVariant InsertationController::isColumnEditable(const int section) const
{
    return m_pModel->headerData(section, Qt::Horizontal, EditableRole);
}

/**
 * @brief InsertationController::modelsPasswordSection
 * @return
 */
int InsertationController::modelsPasswordSection() const
{
    int section = 0;
    for ( ; section<m_pModel->columnCount(); ++section) {
        ModelDataRole role = (ModelDataRole)m_pModel->headerData(section, Qt::Horizontal, DataRoleIdRole).toInt();
        if (role == PasswordRole) {
            break;
        }
    }

    return section;
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
        if (! m_pModel->headerData(column, Qt::Horizontal, EditableRole).toBool()) {
            continue;
        }
        QString role = m_pModel->headerData(column, Qt::Horizontal, DataRoleNameRole).toString();
        QVariant value = modifiedData.value(role, QVariant());
        int roleId = m_pModel->headerData(column, Qt::Horizontal, DataRoleIdRole).toInt();
        QVariant original = m_pModel->data(index, roleId);
        QVariant::Type dataType = m_pModel->headerData(column, Qt::Horizontal, DataTypeRole).type();
        if (value.convert(dataType)) {
            if (value != original) {
                m_pModel->setData(index, value, roleId);
                hasModifications = true;
            }
        }
    }

    return hasModifications;
}
