#ifndef ABSTRACTTABLEVIEWMODEL_H
#define ABSTRACTTABLEVIEWMODEL_H

#include "modelconstants.h"
#include <QAbstractListModel>

class AbstractTableViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AbstractTableViewModel(QObject *parent = 0);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual void resetContent(const QList<QVariantMap> *newContent = NULL) = 0;
    virtual void resetModelRoles() = 0;
    virtual QString modelRoleName(const int role) const = 0;

signals:

public slots:
};

#endif // ABSTRACTTABLEVIEWMODEL_H
