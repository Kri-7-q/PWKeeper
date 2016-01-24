#ifndef ABSTRACTTABLEVIEWMODEL_H
#define ABSTRACTTABLEVIEWMODEL_H

#include "modelconstants.h"
#include <QAbstractListModel>

class AbstractTableViewModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(DataState)
    Q_PROPERTY(bool isModelModified READ isModelModified NOTIFY isModelModifiedChanged)

public:
    explicit AbstractTableViewModel(QObject *parent = 0);

    enum DataState { Origin = 1, New, Modified, Deleted };

    bool isModelModified() const;
    void setModelRowState(const int row, const DataState newState);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
    // Removes the current content of the model. If 'newContent' is not NULL
    // the list content is set into the model as its new content.
    virtual void resetContent(const QList<QVariantMap> *newContent = NULL) = 0;
    // After a model header content was set this function get the model roles
    // out of the header to offer it to the TableView object.
    virtual void resetModelRoles() = 0;
    // Translates a model role to a string. (to query QVariantMap)
    virtual QString modelRoleName(const int role) const = 0;

signals:
    void isModelModifiedChanged();
    void modelRowStateChanged(const DataState state);

public slots:
    DataState modelRowState(const int row) const;

private:
    QHash<int,DataState> m_modelRowState;
};

#endif // ABSTRACTTABLEVIEWMODEL_H
