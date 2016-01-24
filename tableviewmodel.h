#ifndef TABLEMODEL_H
#define TABLEMODEL_H

/* ------------------------------------------------------------------------------------
 * Class TableModel
 *
 * the Applications model which keep all Accounts after they are loaded from
 * persistence.
 * ------------------------------------------------------------------------------------
 */

#include "abstracttableviewmodel.h"
#include <QDebug>


typedef QHash<int, QVariant> HeaderSection;


class TableViewModel : public AbstractTableViewModel
{
    Q_OBJECT

public:
    explicit TableViewModel(QObject *parent = 0);

signals:

public slots:

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::UserRole);
    bool setData(const QModelIndex &index, const QVariant &value, const QString &key);
    QHash<int,QByteArray> roleNames() const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    // Concret implementations
    void resetContent(const QList<QVariantMap> *newContent = NULL);
    void resetModelRoles();
    QString modelRoleName(const int role) const;

private:
    QList<QVariantMap> m_rowList;
    QList<HeaderSection> m_headerList;
    QHash<int, QByteArray> m_modelRoles;

    // Methods
    QHash<int, QByteArray> getModelRoles() const;
    void growHeaderTo(const int section);
};

#endif // TABLEMODEL_H
