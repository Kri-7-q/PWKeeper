#ifndef CONTROLLER_H
#define CONTROLLER_H

/* ---------------------------------------------------------------------
 * Class Controller
 *
 * This class provides elementary funktionality for MVC controllers.
 * ---------------------------------------------------------------------
 */

#include "tablemodel.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TableModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit Controller(QObject *parent = 0);

    void setModel(TableModel* pModel);
    TableModel* model() const;

signals:
    void modelChanged();

public slots:

protected:
    TableModel* m_pModel;
};

#endif // CONTROLLER_H
