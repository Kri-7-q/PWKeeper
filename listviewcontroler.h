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
    void persistModelModifications(const QStringList editableRoles) const;

private slots:
    void setModelContent();

private:
    QList<QVariantMap> m_persistentData;
    Persistence m_database;

    // Private Methods
    QVariantMap newAccountFromModel(const QModelIndex& index, const QStringList& editableRoles) const;
    void updateModelAfterPersist(const QModelIndex& index, const QSqlDatabase &db) const;
};

#endif // LISTVIEWCONTROLER_H
