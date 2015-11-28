#include "tablemodel.h"
#include "Persistence/persistence.h"

// Constructor
TableModel::TableModel(QObject *parent) :
    QAbstractListModel(parent),
    m_isModified(false)
{
    m_roles = Persistence::getTableModelRoles();
    m_roles.insert(StateRole, QString("state").toLocal8Bit());
    initializeHeaderData();
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
 * Private
 * Initialize the Header data of model.
 * Contains meta data to each value of an Account object.
 */
void TableModel::initializeHeaderData()
{
    QVariantMap headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Id")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(IdRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("Id")));
    headerItem.insert(QString("editable"), QVariant(false));
    headerItem.insert(QString("standardValue"), QVariant(QString()));
    headerItem.insert(QString("dataType"), QVariant::Int);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Provider")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(ProviderRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("Amazon")));
    headerItem.insert(QString("editable"), QVariant(true));
    headerItem.insert(QString("standardValue"), QVariant(QString()));
    headerItem.insert(QString("dataType"), QVariant::String);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Benutzername")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(UsernameRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("Horst Krampf")));
    headerItem.insert(QString("editable"), QVariant(true));
    headerItem.insert(QString("standardValue"), QVariant(QString()));
    headerItem.insert(QString("dataType"), QVariant::String);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Passwort")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(PasswordRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("horst_123")));
    headerItem.insert(QString("editable"), QVariant(true));
    headerItem.insert(QString("standardValue"), QVariant(QString()));
    headerItem.insert(QString("dataType"), QVariant::String);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Sicherheitsfrage")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(QuestionRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("Deine lieblings Farbe?")));
    headerItem.insert(QString("editable"), QVariant(true));
    headerItem.insert(QString("standardValue"), QVariant(QString()));
    headerItem.insert(QString("dataType"), QVariant::String);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Antwort")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(AnswerRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("Grün")));
    headerItem.insert(QString("editable"), QVariant(true));
    headerItem.insert(QString("standardValue"), QVariant(QString()));
    headerItem.insert(QString("dataType"), QVariant::String);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Passwortlänge")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(LengthRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("12")));
    headerItem.insert(QString("editable"), QVariant(true));
    headerItem.insert(QString("standardValue"), QVariant(12));
    headerItem.insert(QString("dataType"), QVariant::Int);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Zeichensatz")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(DefinedCharacterRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("*[a-Z]*[0-9]")));
    headerItem.insert(QString("editable"), QVariant(true));
    headerItem.insert(QString("standardValue"), QVariant(QString("*[a-Z]*[0-9]")));
    headerItem.insert(QString("dataType"), QVariant::String);
    m_headerList << headerItem;
    headerItem.insert(QString("headerName"), QVariant(QString("Letzte Änderung")));
    headerItem.insert(QString("roleName"), QVariant(modelRoleName(LastModifyRole)));
    headerItem.insert(QString("placeHolder"), QVariant(QString("Datum")));
    headerItem.insert(QString("editable"), QVariant(false));
    headerItem.insert(QString("standardValue"), QVariant(QString()));
    headerItem.insert(QString("dataType"), QVariant::DateTime);
    m_headerList << headerItem;
}
