#ifndef LISTVIEWCONTROLER_H
#define LISTVIEWCONTROLER_H

#include "Persistence/persistence.h"
#include "controller.h"

class ListViewControler : public Controller
{
    Q_OBJECT

public:
    explicit ListViewControler(QObject *parent = 0);

signals:

public slots:
    void deleteModelRow(const int row) const;

private slots:
    void setModelContent();

private:
    QList<QVariantMap> m_persistentData;
    Persistence m_database;
};

#endif // LISTVIEWCONTROLER_H
