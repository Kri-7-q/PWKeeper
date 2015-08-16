#include "tablemodel.h"

// Constructor
TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_roles.insert(IdRole, QString("id").toLocal8Bit());
    m_roles.insert(ProviderRole, QString("provider").toLocal8Bit());
    m_roles.insert(UsernameRole, QString("username").toLocal8Bit());
    m_roles.insert(PasswordRole, QString("password").toLocal8Bit());
}

// Override
int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowList.size();
}

// Override
int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (m_rowList.size() > 0) {
        return m_rowList[0].size();
    }

    return 0;
}

// Override
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    QString key = mapkeyFromRole(role);
    QVariant value = m_rowList[index.row()].value(key);

    return value;
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
}

// Override
QHash<int, QByteArray> TableModel::roleNames() const
{
    return m_roles;
}

/**
 * @brief TableModel::insertAccount
 * @param account
 */
void TableModel::insertAccount(const QVariantMap &account)
{
    int row = m_rowList.size();
    beginInsertRows(index(row, 0), row, row);
    m_rowList << account;
    endInsertRows();
}

/**
 * PRIVATE
 * Get a role name from model role.
 * All role names are content of a hashmap.
 * @param role      A model role.
 * @return          The role name as a string.
 */
QString TableModel::mapkeyFromRole(int role) const
{
    QByteArray roleNameArray = m_roles.value(role);

    return QString(roleNameArray);
}
