#ifndef LISTVIEWCONTROLER_H
#define LISTVIEWCONTROLER_H

/* ------------------------------------------------------------------------
 * Class ListViewControler
 *
 * Controller of a MVC pattern.
 * This is the controller between the view wich provides a list of
 * account objects and the model.
 * It has access to the persistence too.
 * Base class Controller emit signal 'modelChanged()' when another
 * model was set. This signal is connected to the slot 'setModelContent()'.
 * In that method data is read from persistence an set to the model.
 * Header data is set to the model an signal 'headerDataChanged()' is
 * emitted. This signal is catched by the TableView.
 * ------------------------------------------------------------------------
 */

#include "Persistence/persistencefactory.h"
#include "controller.h"
#include <QDebug>

class ListViewControler : public Controller
{
    Q_OBJECT

public:
    explicit ListViewControler(QObject *parent = 0);
    ~ListViewControler();

signals:
    void modelContentAvailable();
    void selectTableViewRow(const int row);

public slots:
    void deleteModelRow(const int row);
    void persistModelModifications();
    int columnCount() const;
    QVariant isColumnVisible(const int column) const;
    QVariant roleNameOfColumn(const int column) const;

private slots:
    void setModelContent();

private:
    Persistence* m_pPersistence;

    // Private Methods
    QVariantMap accountWithEditableRoles(const int row) const;
    QVariantMap accountObjectFromModel(const int row) const;
    void replaceAccountObject(const int row, const QVariantMap& account);
};

#endif // LISTVIEWCONTROLER_H
