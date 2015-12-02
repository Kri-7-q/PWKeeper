#ifndef TABLEMODEL_H
#define TABLEMODEL_H

/* ------------------------------------------------------------------------------------
 * Class TableModel
 *
 * the Applications model which keep all Accounts after they are loaded from
 * persistence.
 * ------------------------------------------------------------------------------------
 */

#include <QAbstractListModel>
#include <QList>
#include <QVariantMap>
#include <QDebug>

class TableModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(ModelRowState)
    Q_PROPERTY(bool isModified READ isModified WRITE setIsModified NOTIFY isModifiedChanged)

public:
    explicit TableModel(QObject *parent = 0);

    enum Role { StateRole = Qt::UserRole+50 };
    enum ModelRowState { Origin, Modified, Deleted, New };

    void setHeaderContent(const QList<QVariantMap>& headerContent);

signals:
    void isModifiedChanged();
    void dataStyleChanged();
    void headerContentChanged();

public slots:
    TableModel::ModelRowState modelRowState(const int row) const;
    QString modelRoleName(int role) const;
    QVariant data(const int row, const QString &role) const;
    int columnCount() const;
    QVariant headerData(const int section, const QString& role) const;

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant data(const QModelIndex &index, const QString &key) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::UserRole);
    bool setData(const QModelIndex &index, const QVariant &value, const QString &key);
    void resetContent(const QList<QVariantMap> *newContent = NULL);
    QHash<int,QByteArray> roleNames() const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    // Concret class members
    QVariantMap getAccountObject(const int row) const;
    bool isModified() const;
    void setIsModified(bool isModified);
    int appendEmptyRow(const QVariantMap& standardData = QVariantMap());

private:
    QList<QVariantMap> m_rowList;
    QList<QVariantMap> m_headerList;
    QHash<int, QByteArray> m_modelRoles;
    bool m_isModified;

    // Methods
    QHash<int, QByteArray> getModelRoles();
};

#endif // TABLEMODEL_H
