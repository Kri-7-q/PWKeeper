#ifndef CONTROLLER_H
#define CONTROLLER_H

/* ---------------------------------------------------------------------
 * Class Controller
 *
 * This class provides elementary funktionality for MVC controllers.
 * ---------------------------------------------------------------------
 */

#include "abstracttableviewmodel.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AbstractTableViewModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit Controller(QObject *parent = 0);

    void setModel(AbstractTableViewModel *pModel);
    AbstractTableViewModel *model() const;

signals:
    void modelChanged();

public slots:
    QVariant modelData(const int section, const int row) const;
    QVariant modelHeaderText(const int column) const;
    QVariant standardText(const int column) const;

protected:
    AbstractTableViewModel* m_pModel;
};

#endif // CONTROLLER_H
