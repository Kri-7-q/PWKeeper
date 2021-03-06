#ifndef VIEWCONTROLER_H
#define VIEWCONTROLER_H

/* --------------------------------------------------------------------
 * Class ViewControler
 *
 * The controller which switches between the different views.
 * --------------------------------------------------------------------
 */

#include <QObject>

class ViewControler : public QObject
{
    Q_OBJECT
    Q_ENUMS(AppView)
    Q_PROPERTY(AppView currentView READ currentView WRITE setCurrentView NOTIFY currentViewChanged)

public:
    explicit ViewControler(QObject *parent = 0);

    enum AppView { AccountList, ModifyAccount, ShowAccount, NewAccount };

    AppView currentView() const;
    void setCurrentView(const AppView &currentView);

signals:
    void currentViewChanged();

public slots:

private:
    AppView m_currentView;
};

#endif // VIEWCONTROLER_H
