#ifndef INSERTATIONCONTROLLER_H
#define INSERTATIONCONTROLLER_H

#include "controller.h"
#include "Generator/pwgenerator.h"

class InsertationController : public Controller
{
    Q_OBJECT

public:
    explicit InsertationController(QObject *parent = 0);

signals:

public slots:
    void setModifiedData(const int row, const QVariantMap modifiedData, const QStringList roleList);
    void insertNewData(const QVariantMap newData);
    QString generatePassword(const QVariantMap account) const;
};

#endif // INSERTATIONCONTROLLER_H
