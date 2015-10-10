#include "tablemodel.h"

// Constructor
TableModel::TableModel(QObject *parent) :
    QAbstractListModel(parent),
    m_isModified(false)
{
    m_roles.insert(IdRole, QString("id").toLocal8Bit());
    m_roles.insert(ProviderRole, QString("provider").toLocal8Bit());
    m_roles.insert(UsernameRole, QString("username").toLocal8Bit());
    m_roles.insert(PasswordRole, QString("password").toLocal8Bit());
    m_roles.insert(LengthRole, QString("passwordlength").toLocal8Bit());
    m_roles.insert(DefinedCharacterRole, QString("definedcharacter").toLocal8Bit());
    m_roles.insert(AnswerRole, QString("answer").toLocal8Bit());
    m_roles.insert(QuestionRole, QString("question").toLocal8Bit());
    m_roles.insert(LastModifyRole, QString("lastmodify").toLocal8Bit());
    m_roles.insert(StateRole, QString("state").toLocal8Bit());
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
        qDebug() << "Model has not row number : " << index.row();
        return QVariant();
    }

    return m_rowList[index.row()].value(key);
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
    QVariantMap entry = m_rowList[row];
    QVariant state = entry.value(QString("state"), QVariant(TableModel::Origin));

    return (TableModel::ModelRowState)state.toInt();
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
 * Append a new row to the model.
 * @return The number of newly appended row.
 */
int TableModel::appendRow()
{
    int row = m_rowList.size();
    if (! insertRows(row, 1)) {
        return -1;
    }

    return row;
}

/**
 * PRIVATE
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
