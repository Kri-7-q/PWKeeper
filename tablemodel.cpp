#include "tablemodel.h"

// Constructor
TableModel::TableModel(QObject *parent) :
    QAbstractListModel(parent),
    m_isModified(false)
{
}

/**
 * Public
 * Setter - TableModel header content.
 * @param headerContent
 */
void TableModel::setHeaderContent(const QList<QVariantMap> &headerContent)
{
    m_headerList = headerContent;
    m_modelRoles = getModelRoles();
    emit headerContentChanged();
}

// Override
int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowList.size();
}


// Override
QVariant TableModel::data(const QModelIndex &index, const int role) const
{
    QString key = modelRoleName(role);

    return data(index, key);
}

// Overload
QVariant TableModel::data(const QModelIndex &index, const QString &key) const
{
    if (index.row() < 0 || index.row() >= m_rowList.size()) {
        qDebug() << "TableModel:dat() --> Model has not row number : " << index.row();
        return QVariant();
    }
    QVariant value = m_rowList[index.row()].value(key, QVariant(QString()));

    return value;
}

// Override
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QString roleName = modelRoleName(role);

    return setData(index, value, roleName);
}

/**
 * Set data into model.
 * @param index     The model index with row number.
 * @param value     A QVariant value to set into model.
 * @param key       The key to the QVariantMap of model entry.
 * @return          True if model was successfully modified.
 */
bool TableModel::setData(const QModelIndex &index, const QVariant &value, const QString &key)
{
    if (index.row() < 0 || index.row() >= m_rowList.size()) {
        qDebug() << "Model has not row number : " << index.row();
        return false;
    }
    QVariantMap &entry = m_rowList[index.row()];
    entry.insert(key, value);
    emit dataChanged(index, index);
    setIsModified(true);
    if (key == modelRoleName(StateRole)) {
        emit dataStyleChanged();
    }

    return true;
}

// Remove all content. And set new content if parameter is not null.
void TableModel::resetContent(const QList<QVariantMap> *newContent)
{
    beginResetModel();
    m_rowList.clear();
    if (newContent != NULL) {
        m_rowList.append(*newContent);
    }
    endResetModel();
    setIsModified(false);
}

// Override
QHash<int, QByteArray> TableModel::roleNames() const
{
    return m_modelRoles;
}

/**
 * Get the state of a row in TableModel.
 * If a row comes from persistence it do not have a state value.
 * In that case state 'Origin' is returned. New created objects
 * in the model have state 'New'. If an Account object is modified
 * is gets the state 'Modified'. If the User delete an Account
 * it is not removed from model but get the state 'Deleted'.
 * @param row       A row in the model.
 * @return          The current state of that row.
 */
TableModel::ModelRowState TableModel::modelRowState(const int row) const
{
    QModelIndex index = this->index(row);
    QVariant value = data(index, StateRole);
    if (!value.canConvert(QVariant::Int)) {
        return Origin;
    }
    ModelRowState state = (ModelRowState)value.toInt();
    if (state < Origin || state > New) {
        return Origin;
    }

    return state;
}

/**
 * Get a complete row of the model. The whole Account
 * object will be returned in a QVariantMap.
 * @param row   A row in the model.
 * @return      The Account object in that row.
 */
QVariantMap TableModel::getAccountObject(const int row) const
{
    if (row < 0 || row >= m_rowList.size()) {
        qDebug() << "This row does not exist !";
        return QVariantMap();
    }

    return m_rowList[row];
}

// Getter
bool TableModel::isModified() const
{
    return m_isModified;
}

// Setter
void TableModel::setIsModified(bool isModified)
{
    if (m_isModified != isModified) {
        m_isModified = isModified;
        emit isModifiedChanged();
    }
}

// Overwrite
bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > m_rowList.size()) {
        return false;
    }
    beginInsertRows(parent, row, row + count);
    for (int index=row; index<count; ++index) {
        m_rowList.insert(index, QVariantMap());
    }
    endInsertRows();

    return true;
}

/**
 * Public
 * Get a role name from model role enum.
 * All role names are content of a hashmap.
 * @param role      A model role.
 * @return          The role name as a string.
 */
QString TableModel::modelRoleName(int role) const
{
    QByteArray roleNameArray = m_modelRoles.value(role);

    return QString(roleNameArray);
}

/**
 * Slot
 * Overloaded function.
 * @param row       The number of row in model.
 * @param role      The display role name.
 * @return          A QVariant containing the data or an empty QVariant.
 */
QVariant TableModel::data(const int row, const QString &role) const
{
    QModelIndex index = this->index(row);

    return data(index, role);
}

/**
 * Slot
 * Get amount of columns in the model from header data.
 * @return      The number of columns in the model.
 */
int TableModel::columnCount() const
{
    return m_headerList.size();
}

/**
 * Slot
 * Get column header data.
 * @param section   The column number. Zero base.
 * @param role      The header role.
 * @return          A value from header.
 */
QVariant TableModel::headerData(const int section, const QString &role) const
{
    return m_headerList[section].value(role);
}

/**
 * Slot
 * Insert an empty row into model.
 * Should be called before add a new Account.
 * If it is required for some reason the row can be initialized
 * with some data.
 * @param           An empty map or with some standard data.
 * @return          The number of the new row.
 */
int TableModel::appendEmptyRow(const QVariantMap &standardData)
{
    int row = m_rowList.size();
    QModelIndex index = this->index(row);
    beginInsertRows(index, row, row);
    m_rowList << standardData;
    endInsertRows();
    setIsModified(true);

    return row;
}

/**
 * Private
 * Extract model rolesId's and role names from header data.
 * @return
 */
QHash<int, QByteArray> TableModel::getModelRoles()
{
    QHash<int, QByteArray> roles;
    for (int column=0; column<columnCount(); ++column) {
        int roleId = headerData(column, QString("roleId")).toInt();
        QString roleName = headerData(column, QString("roleName")).toString();
        roles.insert(roleId, roleName.toLocal8Bit());
    }
    roles.insert(StateRole, QString("state").toLocal8Bit());

    return roles;
}

/**
 * Override
 * Delete model rows.
 * @param row           The first row which is to delete.
 * @param count         The number of rows to delete.
 * @param parent        The parent model index.
 * @return              True if rows are deleted.
 */
bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int firstRow = row;
    int lastRow = row + count - 1;
    if (firstRow < 0 || lastRow >= m_rowList.size()) {
        qDebug() << "Cannot delete row. It does not exist !";
        return false;
    }
    beginRemoveRows(QModelIndex(), firstRow, lastRow);
    for (int i=0; i<count; ++i) {
        m_rowList.removeAt(row);
    }
    endRemoveRows();

    return true;
}
