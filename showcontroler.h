#ifndef SHOWCONTROLER_H
#define SHOWCONTROLER_H

#include "tablemodel.h"

class ShowControler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TableModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit ShowControler(QObject *parent = 0);

    TableModel *model() const;
    void setModel(TableModel *model);

signals:
    void modelChanged();

public slots:
    QVariantMap modelRowEntry(const int row) const;

private:
    TableModel* m_pModel;
};

#endif // SHOWCONTROLER_H
