#ifndef LISTVIEWCONTROLER_H
#define LISTVIEWCONTROLER_H

#include "Persistence/persistencefactory.h"
#include "controller.h"

class ListViewControler : public Controller
{
    Q_OBJECT

public:
    explicit ListViewControler(QObject *parent = 0);
    ~ListViewControler();

signals:

public slots:
    void deleteModelRow(const int row) const;
    void persistModelModifications() const;

private slots:
    void setModelContent();

private:
    Persistence* m_pPersistence;

    // Private Methods
    QVariantMap accountWithEditableRoles(const QModelIndex& index) const;
};

#endif // LISTVIEWCONTROLER_H
