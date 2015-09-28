#include "tablemodel.h"

// Constructor
TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_roles.insert(IdRole, QString("id").toLocal8Bit());
    m_roles.insert(ProviderRole, QString("provider").toLocal8Bit());
    m_roles.insert(UsernameRole, QString("username").toLocal8Bit());
    m_roles.insert(PasswordRole, QString("password").toLocal8Bit());
    m_roles.insert(LengthRole, QString("length").toLocal8Bit());
    m_roles.insert(DefinedCharacterRole, QString("definedcharacter").toLocal8Bit());
    m_roles.insert(AnswerRole, QString("answer").toLocal8Bit());
    m_roles.insert(QuestionRole, QString("question").toLocal8Bit());
    m_roles.insert(LastModifyRole, QString("lastmodify").toLocal8Bit());
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
QVariant TableModel::data(const QModelIndex &index, const int role) const
{
    QString key = mapkeyFromRole(role);
    QVariant value = m_rowList[index.row()].value(key);

    return value;
}

// Overload
QVariant TableModel::data(const QModelIndex &index, const QString &key) const
{
    return m_rowList[index.row()].value(key);
}

// Override
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QString roleName = mapkeyFromRole(role);

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
    if (index.row() < 0 || index.row() > m_rowList.size()) {
        return false;
    }
    QVariantMap &entry = m_rowList[index.row()];
    entry.insert(key, value);
    emit dataChanged(index, index);

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
    QVariantMap entry = m_rowList[row];
    QVariant state = entry.value(QString("state"), QVariant(TableModel::Origin));

    return (TableModel::ModelRowState)state.toInt();
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
 * @brief TableModel::getRow
 * @param row
 * @return
 */
QVariantMap TableModel::getRow(const int row) const
{
    return m_rowList[row];
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
