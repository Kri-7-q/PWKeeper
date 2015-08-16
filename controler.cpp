#include "controler.h"
#include <QClipboard>
#include <QApplication>

Controler::Controler(TableModel *model, QObject *parent) :
    QObject(parent),
    m_model(model)
{
    try {
    m_databaseContent = m_persistence.readWholeTable(m_persistence.tableName());
    showAllData();
    } catch (SqlException exception) {
        qDebug() << "Error : " << exception.errorText();
        qDebug() << "Query : " << exception.sqlStatement();
    }
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
