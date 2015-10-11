#include "addcontroler.h"

AddControler::AddControler(QObject *parent) :
    InsertationController(parent)
{
    initializeDialogModel();
}


/**
  * Property
  * Get an emty model for dialog controls.
  * @return      A QVariantMap map with emty strings in QVariant objects
  *              for each control in a dialog.
  */
QVariantMap AddControler::dialogModel() const
{
    return m_dialogModel;
}

/**
  * Private
  * Initialize the model for dialog controls.
  * Set an emty string to all values.
  */
void AddControler::initializeDialogModel()
{
    QStringList keyList;
    keyList << "id" << "provider" << "username" << "password" << "question" << "answer" << "lastmodify";
    foreach (QString key, keyList) {
        m_dialogModel.insert(key, QVariant(QString()));
    }
    m_dialogModel.insert(QString("passwordlength"), QVariant(12));
    m_dialogModel.insert(QString("definedcharacter"), QVariant(QString("*[A-Z]*[a-z]*[0-9]")));
}
