#ifndef CONTROLER_H
#define CONTROLER_H

#include "tablemodel.h"
#include "persistence.h"
#include <QQuickItem>

class Controler : public QObject
{
    Q_OBJECT
    Q_ENUMS(AppView)
    Q_PROPERTY(bool dataModified READ dataModified NOTIFY dataModifiedChanged)
    Q_PROPERTY(AppView currentView READ currentView WRITE setCurrentView NOTIFY currentModeChanged)
    Q_PROPERTY(TableModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit Controler(QObject *parent = 0);

    // Constants
    enum AppView { AccountList, ShowAccount, ModifyAccount };

    // Getter und Setter
    bool dataModified() const;
    void setDataModified(const bool modifyFlag);
    AppView currentView() const;
    void setCurrentView(const AppView view);
    void setModel(TableModel *model);
    TableModel* model() const;
    int currentTableViewRow() const;
    void setCurrentTableViewRow(int currentTableViewRow);

signals:
    void dataModifiedChanged();
    void currentModeChanged();
    void modelChanged();
    void currentTableViewRowChanged();

public slots:
    void findValuesFor(const QString &type, const QString &value) const;
    void showAllData() const;
    void readFromDatabase(const QVariantMap &searchParameter) const;
    void copyPasswordToClipboard(const int currentRow) const;
    QVariantMap modelRowEntry(const int row) const;
    void setModifiedData(const QVariantMap &data);

private:
    Persistence m_persistence;
    TableModel* m_model;
    QList<QVariantMap> m_databaseContent;
    bool m_dataModified;
    AppView m_currentView;
    int m_currentTableViewRow;

    // Methods
    QVariantMap getSearchParameter(const QStringList &columnList) const;
};

#endif // CONTROLER_H
