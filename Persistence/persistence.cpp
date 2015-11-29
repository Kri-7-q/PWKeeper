#include "persistence.h"

/**
 * Constructor
 */
Persistence::Persistence() :
    m_isOpen(false)
{

}

/**
 * Virtual public
 * Returns if the persistence is open to read and write or not.
 * Method 'setOpen(bool)' must be used in 'open()' and 'close()'
 * otherwise this methode returns allways false;
 * @return          True if persistence is open. Otherwise false.
 */
bool Persistence::isOpen() const
{
    return m_isOpen;
}

/**
 * Virtual publc
 * Methode should provied header data for the TableModel and TableView.
 * There are some values for each column to provide.
 * One column MUST have:
 * -----------------------
 * headerName           The name of the value in the column.
 * roleName             The role name which is taken in GUI to get data from the model.
 *                      Must also used by the QVariantMap which contains Account object.
 *                      For instance:
 *                      roleName: id        value: 12
 *                      roleName: provider  value: Amazon
 * roleId               An int value starting with Qt::UserRole enum. (Model role)
 * placeHolder          A string which is used as placeholder in input fields.
 * editable             True if a value should be editable by the user. Otherwise false.
 * standardValue        A value to fill in the input field as standard value or nothing 'QVariant()'.
 * dataType             The data type which should the value have. One of QVariant types.
 * @return
 */
QList<QVariantMap> Persistence::getModelHeader() const
{
    QList<QVariantMap> modelHeader;
    QVariantMap columnHeader;
    columnHeader.insert(QString("headerName"), QString("Id"));
    columnHeader.insert(QString("roleName"), QString("id"));
    columnHeader.insert(QString("roleId"), Qt::UserRole);
    columnHeader.insert(QString("placeHolder"), QString("Primary Key"));
    columnHeader.insert(QString("editable"), QVariant(false));
    columnHeader.insert(QString("standardValue"), QVariant(0));
    columnHeader.insert(QString("dataType"), QVariant::Int);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Provider"));
    columnHeader.insert(QString("roleName"), QString("provider"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 1);
    columnHeader.insert(QString("placeHolder"), QString("Amazon"));
    columnHeader.insert(QString("editable"), QVariant(true));
    columnHeader.insert(QString("standardValue"), QVariant());
    columnHeader.insert(QString("dataType"), QVariant::String);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Benutzername"));
    columnHeader.insert(QString("roleName"), QString("username"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 2);
    columnHeader.insert(QString("placeHolder"), QString("Horst Krampf"));
    columnHeader.insert(QString("editable"), QVariant(true));
    columnHeader.insert(QString("standardValue"), QVariant());
    columnHeader.insert(QString("dataType"), QVariant::String);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Passwort"));
    columnHeader.insert(QString("roleName"), QString("password"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 3);
    columnHeader.insert(QString("placeHolder"), QString("Passwort"));
    columnHeader.insert(QString("editable"), QVariant(true));
    columnHeader.insert(QString("standardValue"), QVariant());
    columnHeader.insert(QString("dataType"), QVariant::String);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Passwortlänge"));
    columnHeader.insert(QString("roleName"), QString("passwordlength"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 4);
    columnHeader.insert(QString("placeHolder"), QString("Länge des Passworts"));
    columnHeader.insert(QString("editable"), QVariant(true));
    columnHeader.insert(QString("standardValue"), QVariant(12));
    columnHeader.insert(QString("dataType"), QVariant::Int);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Sicherheitsfrage"));
    columnHeader.insert(QString("roleName"), QString("question"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 5);
    columnHeader.insert(QString("placeHolder"), QString("Deine lieblings Farbe?"));
    columnHeader.insert(QString("editable"), QVariant(true));
    columnHeader.insert(QString("standardValue"), QVariant());
    columnHeader.insert(QString("dataType"), QVariant::String);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Antwort"));
    columnHeader.insert(QString("roleName"), QString("answer"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 6);
    columnHeader.insert(QString("placeHolder"), QString("Grün"));
    columnHeader.insert(QString("editable"), QVariant(true));
    columnHeader.insert(QString("standardValue"), QVariant());
    columnHeader.insert(QString("dataType"), QVariant::String);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Zeichensatz"));
    columnHeader.insert(QString("roleName"), QString("definedcharacter"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 7);
    columnHeader.insert(QString("placeHolder"), QString("Zeichen für Passwort"));
    columnHeader.insert(QString("editable"), QVariant(true));
    columnHeader.insert(QString("standardValue"), QString("*[a-Z]*[0-9]"));
    columnHeader.insert(QString("dataType"), QVariant::String);
    modelHeader << columnHeader;
    columnHeader.insert(QString("headerName"), QString("Letzte Änderung"));
    columnHeader.insert(QString("roleName"), QString("lastmodify"));
    columnHeader.insert(QString("roleId"), Qt::UserRole + 8);
    columnHeader.insert(QString("placeHolder"), QString("Datum"));
    columnHeader.insert(QString("editable"), QVariant(false));
    columnHeader.insert(QString("standardValue"), QVariant());
    columnHeader.insert(QString("dataType"), QVariant::DateTime);
    modelHeader << columnHeader;

    return modelHeader;
}

/**
 * Virtual public
 * Return the role name of the primary key.
 * Should be model role name and QVariantMap key.
 * @return      The role name of primary key value.
 */
QString Persistence::primaryKeyRoleName() const
{
    return QString("id");
}
