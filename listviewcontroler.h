#ifndef LISTVIEWCONTROLER_H
#define LISTVIEWCONTROLER_H

#include "Persistence/persistence.h"
#include "tablemodel.h"

class ListViewControler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TableModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit ListViewControler(QObject *parent = 0);

    TableModel *model() const;
    void setModel(TableModel *model);

signals:
    void modelChanged();

public slots:
    void deleteModelRow(const int row) const;

private:
    TableModel* m_pModel;
    QList<QVariantMap> m_persistentData;
    Persistence m_database;
};

#endif // LISTVIEWCONTROLER_H
