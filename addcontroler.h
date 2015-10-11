#ifndef ADDCONTROLER_H
#define ADDCONTROLER_H

#include "insertationcontroller.h"

class AddControler : public InsertationController
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap dialogModel READ dialogModel NOTIFY dialogModelChanged)

public:
    explicit AddControler(QObject *parent = 0);

    QVariantMap dialogModel() const;

signals:
    void dialogModelChanged();

public slots:

protected:
    QVariantMap m_dialogModel;

    // Private methods
    void initializeDialogModel();
};

#endif // ADDCONTROLER_H
