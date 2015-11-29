#ifndef LISTVIEWCONTROLER_H
#define LISTVIEWCONTROLER_H

#include "Persistence/psqldatabase.h"
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
    PSqlDatabase m_database;

    // Private Methods
    QVariantMap newAccountFromModel(const QModelIndex& index, const QStringList& editableRoles) const;
    QVariantMap modifications(const QModelIndex& index, const QVariantMap& origin) const;
};

#endif // LISTVIEWCONTROLER_H
