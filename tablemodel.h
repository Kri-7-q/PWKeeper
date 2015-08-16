#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariantMap>
#include <QDebug>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = 0);

    enum Roles { IdRole = Qt::UserRole, ProviderRole, UsernameRole, PasswordRole };

signals:

public slots:

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void resetContent(const QList<QVariantMap> *newContent = NULL);
    QHash<int,QByteArray> roleNames() const;

    // Insert data
    void insertAccount(const QVariantMap &account);

private:
    QList<QVariantMap> m_rowList;
    QHash<int, QByteArray> m_roles;

    // Private methods
    QString mapkeyFromRole(int role) const;
};

#endif // TABLEMODEL_H
