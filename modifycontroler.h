#ifndef MODIFYCONTROLER_H
#define MODIFYCONTROLER_H

#include "tablemodel.h"
#include "pwgenerator.h"

class ModifyControler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TableModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit ModifyControler(QObject *parent = 0);

    TableModel *model() const;
    void setModel(TableModel *model);

signals:
    void modelChanged();

public slots:
    QVariantMap modelRowEntry(const int row) const;
    void setModifiedData(const int row, const QVariantMap modifiedData, const QStringList roleList);
    QString generatePassword(const QVariantMap account) const;

private:
    TableModel* m_pModel;
};

#endif // MODIFYCONTROLER_H
