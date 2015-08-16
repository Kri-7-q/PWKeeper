#ifndef CONTROLER_H
#define CONTROLER_H

#include "tablemodel.h"
#include "persistence.h"

class Controler : public QObject
{
    Q_OBJECT
public:
    explicit Controler(TableModel *model, QObject *parent = 0);

signals:

public slots:
    void findValuesFor(const QString &type, const QString &value) const;
    void showAllData() const;
    void readFromDatabase(const QVariantMap &searchParameter) const;
    void copyPasswordToClipboard(const int currentRow) const;

private:
    Persistence m_persistence;
    TableModel* m_model;
    QList<QVariantMap> m_databaseContent;

    // Methods
    QVariantMap getSearchParameter(const QStringList &columnList) const;
};

#endif // CONTROLER_H
