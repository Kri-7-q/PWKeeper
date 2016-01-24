#include "tableviewmodel.h"

// Constructor
TableViewModel::TableViewModel(QObject *parent) :
    AbstractTableViewModel(parent)
{
}

// Override
int TableViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowList.size();
}

// Override
int TableViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_headerList.size();
}


// Override
QVariant TableViewModel::data(const QModelIndex &index, const int role) const
{
    QString key = modelRoleName(role);
    if (index.row() < 0 || index.row() >= m_rowList.size()) {
        qDebug() << "TableModel:data() --> Model has not row number : " << index.row();
        return QVariant();
    }
    QVariant value = m_rowList[index.row()].value(key, QVariant());

    return value;
}

// Override
QVariant TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0 || section >= m_headerList.size() || orientation != 1) {
        return QVariant();
    }
    HeaderSection headerSection = m_headerList[section];

    return headerSection.value(role, QVariant());
}

// Override
bool TableViewModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation != Qt::Horizontal || section < 0) {
        return false;
    }
    if (section >= m_headerList.size()) {
        growHeaderTo(section);
    }
    HeaderSection& sectionObj = m_headerList[section];
    sectionObj.insert(role, value);
    emit headerDataChanged(Qt::Horizontal, section, section);

    return true;
}

// Override
bool TableViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
bool TableViewModel::setData(const QModelIndex &index, const QVariant &value, const QString &key)
{
    if (index.row() < 0 || index.row() >= m_rowList.size()) {
        qDebug() << "Model has not row number : " << index.row();
        return false;
    }
    QVariantMap &entry = m_rowList[index.row()];
    entry.insert(key, value);
    emit dataChanged(index, index);

    return true;
}

// Remove all content. And set new content if parameter is not null.
void TableViewModel::resetContent(const QList<QVariantMap> *newContent)
{
    beginResetModel();
    m_rowList.clear();
    if (newContent != NULL) {
        m_rowList.append(*newContent);
    }
    endResetModel();
}

/**
 * Get the model roles out of the header data.
 */
void TableViewModel::resetModelRoles()
{
    m_modelRoles = getModelRoles();
}

// Override
QHash<int, QByteArray> TableViewModel::roleNames() const
{
    return m_modelRoles;
}

// Overwrite
bool TableViewModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > m_rowList.size()) {
        return false;
    }
    int lastRow = row + count -1;
    beginInsertRows(parent, row, lastRow);
    for (int index=row; index<=lastRow; ++index) {
        m_rowList.insert(index, QVariantMap());
    }
    endInsertRows();

    return true;
}

/**
 * Private
 * If a value is to be set to a section which is not in the current range
 * of the header data list then the list must grow to that amount of sections.
 * @param section       The number of section which is out of current range.
 */
void TableViewModel::growHeaderTo(const int section)
{
    int addAmount = section - (m_headerList.size() - 1);
    for (int i=0; i<addAmount; ++i) {
        m_headerList << QHash<int, QVariant>();
    }
}

/**
 * Get a role name from a model role constant.
 * @param role      A model role (enum).
 * @return          The name of the model role. Used for QVariantMap.
 */
QString TableViewModel::modelRoleName(const int role) const
{
    QByteArray roleName = m_modelRoles.value(role);

    return QString(roleName);
}

/**
 * Private
 * Extract model rolesId's and role names from header data.
 * Model role enums and names to provide it through method
 * 'roleName()' to the QML GUI.
 * @return
 */
QHash<int, QByteArray> TableViewModel::getModelRoles() const
{
    QHash<int, QByteArray> roles;
    for (int column=0; column<columnCount(); ++column) {
        int roleId = headerData(column, Qt::Horizontal, DataRoleIdRole).toInt();
        QString roleName = headerData(column, Qt::Horizontal, DataRoleNameRole).toString();
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
bool TableViewModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int firstRow = row;
    int lastRow = row + count - 1;
    if (firstRow < 0 || lastRow >= m_rowList.size()) {
        qDebug() << "Cannot delete row. It does not exist !";
        return false;
    }
    beginRemoveRows(parent, firstRow, lastRow);
    for (int i=0; i<count; ++i) {
        m_rowList.removeAt(row);
    }
    endRemoveRows();

    return true;
}
