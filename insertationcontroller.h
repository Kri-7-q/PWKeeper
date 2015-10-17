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
    void errorMessage(const QString errorMsg);

public slots:
    void setModifiedData(const int row, const QVariantMap modifiedData, const QStringList roleList);
    void insertNewData(const int row, const QVariantMap newData, const QStringList roleList);
    QString generatePassword(const QVariantMap account);

protected:
    bool insertModifiedData(const QModelIndex &index, const QVariantMap &modifiedData, const QStringList &roleList) const;
};

#endif // INSERTATIONCONTROLLER_H
