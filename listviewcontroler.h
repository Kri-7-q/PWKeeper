#ifndef LISTVIEWCONTROLER_H
#define LISTVIEWCONTROLER_H

/* ------------------------------------------------------------------------
 * Class ListViewControler
 *
 * Controller of a MVC pattern.
 * This is the controller between the view wich provides a list of
 * account objects and the model.
 * It has access to the persistence too.
 * ------------------------------------------------------------------------
 */

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
