#ifndef TABLEMODEL_H
#define TABLEMODEL_H

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

    enum Roles { IdRole = Qt::UserRole, ProviderRole, UsernameRole, PasswordRole, LengthRole, DefinedCharacterRole,
                 AnswerRole, QuestionRole, LastModifyRole, StateRole };
    enum ModelRowState { Origin, Modified, Deleted, New };

signals:
    void isModifiedChanged();

public slots:
    TableModel::ModelRowState modelRowState(const int row) const;
    QString modelRoleName(int role) const;

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, const int role) const;
    QVariant data(const QModelIndex &index, const QString &key) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = ProviderRole);
    bool setData(const QModelIndex &index, const QVariant &value, const QString &key);
    void resetContent(const QList<QVariantMap> *newContent = NULL);
    QHash<int,QByteArray> roleNames() const;

    // Read data
    QVariantMap getRow(const int row) const;
    bool isModified() const;
    void setIsModified(bool isModified);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int appendRow();

private:
    QList<QVariantMap> m_rowList;
    QHash<int, QByteArray> m_roles;
    bool m_isModified;
};

#endif // TABLEMODEL_H
