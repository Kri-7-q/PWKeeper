#include "viewcontroler.h"

ViewControler::ViewControler(QObject *parent) :
    QObject(parent),
    m_currentView(AccountList)
{

}

// Getter
ViewControler::AppView ViewControler::currentView() const
{
    return m_currentView;
}

// Setter
void ViewControler::setCurrentView(const AppView &currentView)
{
    if (m_currentView != currentView) {
        m_currentView = currentView;
        emit currentViewChanged();
    }
}

