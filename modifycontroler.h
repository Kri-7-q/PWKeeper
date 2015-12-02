#ifndef MODIFYCONTROLER_H
#define MODIFYCONTROLER_H

/* --------------------------------------------------------------------
 * Class ModifyControler
 *
 * The controller between the view where an Account object can be
 * modified and the model.
 * It follows the MVC pattern.
 * --------------------------------------------------------------------
 */

#include "insertationcontroller.h"

class ModifyControler : public InsertationController
{
    Q_OBJECT

public:
    explicit ModifyControler(QObject *parent = 0);

signals:

public slots:
};

#endif // MODIFYCONTROLER_H
