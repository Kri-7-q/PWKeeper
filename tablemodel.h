#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariantMap>
#include <QDebug>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_ENUMS(ModelRowState)

public:
    explicit TableModel(QObject *parent = 0);

    enum Roles { IdRole = Qt::UserRole, ProviderRole, UsernameRole, PasswordRole, LengthRole, DefinedCharacterRole,
                 AnswerRole, QuestionRole, LastModifyRole };
    enum ModelRowState { Origin, Modified, Deleted };

signals:

public slots:
    TableModel::ModelRowState modelRowState(const int row) const;

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, const int role) const;
    QVariant data(const QModelIndex &index, const QString &key) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = ProviderRole);
    bool setData(const QModelIndex &index, const QVariant &value, const QString &key);
    void resetContent(const QList<QVariantMap> *newContent = NULL);
    QHash<int,QByteArray> roleNames() const;

    // Insert data
    void insertAccount(const QVariantMap &account);
    // Read data
    QVariantMap getRow(const int row) const;

private:
    QList<QVariantMap> m_rowList;
    QHash<int, QByteArray> m_roles;

    // Private methods
    QString mapkeyFromRole(int role) const;
};

#endif // TABLEMODEL_H
