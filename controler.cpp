#include "controler.h"
#include <QClipboard>
#include <QApplication>

Controler::Controler(QObject *parent) :
    QObject(parent),
    m_dataModified(false),
    m_currentView(AccountList)
{
    try {
    m_databaseContent = m_persistence.readWholeTable(m_persistence.tableName());
    } catch (SqlException exception) {
        qDebug() << "Error : " << exception.errorText();
        qDebug() << "Query : " << exception.sqlStatement();
    }
}

// Getter - modified flag
bool Controler::dataModified() const
{
    return m_dataModified;
}

// Setter - modified flag
void Controler::setDataModified(bool dataModified)
{
    if (m_dataModified != dataModified) {
        m_dataModified = dataModified;
        emit dataModifiedChanged();
        emit controlBarButtonChanged();
    }
}

// Getter - AppMode (determines the current view)
Controler::AppView Controler::currentView() const
{
    return m_currentView;
}

// Setter - AppMode (determines the current view)
void Controler::setCurrentView(const Controler::AppView view)
{
    if (m_currentView != view) {
        m_currentView = view;
        emit currentModeChanged();
        emit controlBarButtonChanged();
    }
}

// Setter - model
void Controler::setModel(TableModel *model)
{
    if (m_model != model) {
        m_model = model;
        showAllData();
        emit modelChanged();
    }
}

// Getter - model
TableModel *Controler::model() const
{
    return m_model;
}

/**
 * @brief Controler::findValuesFor
 * @param type
 * @param value
 */
void Controler::findValuesFor(const QString &type, const QString &value) const
{
    QList<QVariantMap> found;
    foreach (QVariantMap account, m_databaseContent) {
        QString content = account.value(type.toLower(), QString()).toString();
        if (content.contains(value)) {
            found << account;
        }
    }
    m_model->resetContent(&found);
}

/**
 * @brief Controler::showAllData
 */
void Controler::showAllData() const
{
    m_model->resetContent(&m_databaseContent);
}

/**
 * @brief Controler::readAllValues
 */
void Controler::readFromDatabase(const QVariantMap &searchParameter) const
{
    try {
        QList<QVariantMap> accountList = m_persistence.findAccounts(searchParameter);
        m_model->resetContent(&accountList);
    } catch(SqlException exception) {
        qDebug() << "Error : " << exception.errorText();
        qDebug() << "Statement : " << exception.sqlStatement();
    }
}

/**
 * @brief Controler::copyPasswordToClipboard
 * @param currentRow
 */
void Controler::copyPasswordToClipboard(const int currentRow) const
{
    QVariant value = m_model->data(m_model->index(currentRow, 0), TableModel::PasswordRole);
    QString password = value.toString();
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(password);
}

/**
 * @brief Controler::modelRowEntry
 * @param row
 * @return
 */
QVariantMap Controler::modelRowEntry(const int row) const
{
    return m_model->getRow(row);
}

/**
 * @brief Controler::setModifiedData
 * @param data
 */
void Controler::setModifiedData(const QVariantMap &data)
{
    if (data.isEmpty()) {
        qDebug() << "There is NO data !!!";
    }
    qDebug() << data;
}

/**
 * @brief Controler::currentTableViewRow
 * @return
 */
int Controler::currentTableViewRow() const
{
    return m_currentTableViewRow;
}

/**
 * @brief Controler::setCurrentTableViewRow
 * @param currentTableViewRow
 */
void Controler::setCurrentTableViewRow(int currentTableViewRow)
{
    if (m_currentTableViewRow != currentTableViewRow) {
        m_currentTableViewRow = currentTableViewRow;
        emit currentTableViewRowChanged();
        emit controlBarButtonChanged();
    }
}


/**
 * @brief Controler::getSearchParameterForWholeTable
 * @param tableName
 * @return
 */
QVariantMap Controler::getSearchParameter(const QStringList &columnList) const
{
    QVariantMap searchParameter;
    foreach (QString columnName, columnList) {
        searchParameter.insert(columnName, QVariant());
    }

    return searchParameter;
}
