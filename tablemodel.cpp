#include "tablemodel.h"
#include "Persistence/persistence.h"

// Constructor
TableModel::TableModel(QObject *parent) :
    QAbstractListModel(parent),
    m_isModified(false)
{
    m_roles = Persistence::getTableModelRoles();
    m_roles.insert(StateRole, QString("state").toLocal8Bit());
    initializeDataTypeMap();
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
    return m_roles;
}

/**
 * @brief TableModel::modelRowState
 * @param row
 * @return
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
 * @brief TableModel::getRow
 * @param row
 * @return
 */
QVariantMap TableModel::getRow(const int row) const
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
 * Get a role name from model role.
 * All role names are content of a hashmap.
 * @param role      A model role.
 * @return          The role name as a string.
 */
QString TableModel::modelRoleName(int role) const
{
    QByteArray roleNameArray = m_roles.value(role);

    return QString(roleNameArray);
}

/**
 * Slot
 * Overload of function.
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
 * Insert an empty row into model.
 * Should be called before add a new Account.
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

/**
 * Public
 * Get data type to a role name.
 * @param roleName      The name of a model role.
 * @return              A QVariant::Type. QVariant::Invalid if role name is unknown.
 */
QVariant::Type TableModel::dataTypeOfRole(const QString &roleName) const
{
    return m_dataTypeMap.value(roleName, QVariant::Invalid);
}

/**
 * Public
 * Setter - Set data type map.
 * Data type information map to the model roles.
 * @param dataTypeMap
 */
void TableModel::setDataTypeMap(const QHash<QString, QVariant::Type> &dataTypeMap)
{
    m_dataTypeMap = dataTypeMap;
}

/**
 * Private
 * Initialize data type map. Map has role names as key
 * and QVariant::Type as value. These are the database
 * types.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Should be read from database.
 */
void TableModel::initializeDataTypeMap()
{
    m_dataTypeMap.insert(QString("id"), QVariant::Int);
    m_dataTypeMap.insert(QString("provider"), QVariant::String);
    m_dataTypeMap.insert(QString("username"), QVariant::String);
    m_dataTypeMap.insert(QString("password"), QVariant::String);
    m_dataTypeMap.insert(QString("question"), QVariant::String);
    m_dataTypeMap.insert(QString("answer"), QVariant::String);
    m_dataTypeMap.insert(QString("passwordlength"), QVariant::Int);
    m_dataTypeMap.insert(QString("definedcharacter"), QVariant::String);
    m_dataTypeMap.insert(QString("lastmodify"), QVariant::DateTime);
}
