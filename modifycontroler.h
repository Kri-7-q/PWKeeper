#ifndef MODIFYCONTROLER_H
#define MODIFYCONTROLER_H

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
