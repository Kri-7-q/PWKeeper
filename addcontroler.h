#ifndef ADDCONTROLER_H
#define ADDCONTROLER_H

#include "insertationcontroller.h"

class AddControler : public InsertationController
{
    Q_OBJECT

public:
    explicit AddControler(QObject *parent = 0);

signals:

public slots:
    int appendEmptyModelRow() const;
};

#endif // ADDCONTROLER_H
