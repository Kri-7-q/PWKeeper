#include "persistence.h"

/**
 * Constructor
 */
Persistence::Persistence() :
    m_isOpen(false)
{

}

/**
 * Virtual
 * Destructor
 */
Persistence::~Persistence()
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

/*! Virtual publc
 *
 * Methode should provied header data for the TableModel and TableView.
 * There are some values for each column to provide. Each column needs
 * a QVariantMap with meta data to the columns content.
 * One column MUST have:
 * -----------------------
 * - headerName
 *      The name of the value in the column.
 * - roleName
 *      The role name which is taken in GUI to get data from the model.
 *      Must also used by the QVariantMap which contains Account object.
 *      For instance:
 *      roleName: id        value: 12
 *      roleName: provider  value: Amazon
 * - roleId
 *      An int value starting with Qt::UserRole enum. (Model role)
 * - placeHolder
 *      A string which is used as placeholder in input fields.
 * - editable
 *      True if a value should be editable by the user. Otherwise false.
 * - standardValue
 *      A value to fill in the input field as standard value or nothing 'QVariant(type)'.
 * - dataType
 *      The data type which should the value have. One of QVariant types.
 * - showColumn
 *      True if this column should be shown in TableView.
 * @return  A list with QVariantMap objects containing meta data for the TableView.
 */
bool Persistence::setModelHeader(QAbstractListModel *pModel) const
{
    pModel->setHeaderData(0, Qt::Horizontal, QString("id"), HeaderNameRole);
    pModel->setHeaderData(0, Qt::Horizontal, QString("id"), DataRoleNameRole);
    pModel->setHeaderData(0, Qt::Horizontal, QVariant(PrimaryKeyRole), DataRoleIdRole);
    pModel->setHeaderData(0, Qt::Horizontal, QString("Primary Key"), PlaceHolderRole);
    pModel->setHeaderData(0, Qt::Horizontal, QVariant(false), EditableRole);
    pModel->setHeaderData(0, Qt::Horizontal, QVariant(0), StandardValueRole);
    pModel->setHeaderData(0, Qt::Horizontal, QVariant(QVariant::Int), DataTypeRole);
    pModel->setHeaderData(0, Qt::Horizontal, QVariant(false), ColumnVisibleRole);

    pModel->setHeaderData(1, Qt::Horizontal, QString("Provider"), HeaderNameRole);
    pModel->setHeaderData(1, Qt::Horizontal, QString("provider"), DataRoleNameRole);
    pModel->setHeaderData(1, Qt::Horizontal, QVariant(ProviderRole), DataRoleIdRole);
    pModel->setHeaderData(1, Qt::Horizontal, QString("Amazon"), PlaceHolderRole);
    pModel->setHeaderData(1, Qt::Horizontal, QVariant(true), EditableRole);
    pModel->setHeaderData(1, Qt::Horizontal, QVariant(QVariant::String), StandardValueRole);
    pModel->setHeaderData(1, Qt::Horizontal, QVariant(QVariant::String), DataTypeRole);
    pModel->setHeaderData(1, Qt::Horizontal, QVariant(true), ColumnVisibleRole);

    pModel->setHeaderData(2, Qt::Horizontal, QString("Benutzername"), HeaderNameRole);
    pModel->setHeaderData(2, Qt::Horizontal, QString("username"), DataRoleNameRole);
    pModel->setHeaderData(2, Qt::Horizontal, QVariant(UsernameRole), DataRoleIdRole);
    pModel->setHeaderData(2, Qt::Horizontal, QString("Horst Krampf"), PlaceHolderRole);
    pModel->setHeaderData(2, Qt::Horizontal, QVariant(true), EditableRole);
    pModel->setHeaderData(2, Qt::Horizontal, QVariant(QVariant::String), StandardValueRole);
    pModel->setHeaderData(2, Qt::Horizontal, QVariant(QVariant::String), DataTypeRole);
    pModel->setHeaderData(2, Qt::Horizontal, QVariant(true), ColumnVisibleRole);

    pModel->setHeaderData(3, Qt::Horizontal, QString("Passwort"), HeaderNameRole);
    pModel->setHeaderData(3, Qt::Horizontal, QString("password"), DataRoleNameRole);
    pModel->setHeaderData(3, Qt::Horizontal, QVariant(PasswordRole), DataRoleIdRole);
    pModel->setHeaderData(3, Qt::Horizontal, QString("Passwort"), PlaceHolderRole);
    pModel->setHeaderData(3, Qt::Horizontal, QVariant(true), EditableRole);
    pModel->setHeaderData(3, Qt::Horizontal, QVariant(QVariant::String), StandardValueRole);
    pModel->setHeaderData(3, Qt::Horizontal, QVariant(QVariant::String), DataTypeRole);
    pModel->setHeaderData(3, Qt::Horizontal, QVariant(false), ColumnVisibleRole);

    pModel->setHeaderData(4, Qt::Horizontal, QString("Passwortlänge"), HeaderNameRole);
    pModel->setHeaderData(4, Qt::Horizontal, QString("passwordlength"), DataRoleNameRole);
    pModel->setHeaderData(4, Qt::Horizontal, QVariant(PasswordLengthRole), DataRoleIdRole);
    pModel->setHeaderData(4, Qt::Horizontal, QString("Länge des Passworts"), PlaceHolderRole);
    pModel->setHeaderData(4, Qt::Horizontal, QVariant(true), EditableRole);
    pModel->setHeaderData(4, Qt::Horizontal, QVariant(12), StandardValueRole);
    pModel->setHeaderData(4, Qt::Horizontal, QVariant(QVariant::Int), DataTypeRole);
    pModel->setHeaderData(4, Qt::Horizontal, QVariant(false), ColumnVisibleRole);

    pModel->setHeaderData(5, Qt::Horizontal, QString("Sicherheitsfrage"), HeaderNameRole);
    pModel->setHeaderData(5, Qt::Horizontal, QString("question"), DataRoleNameRole);
    pModel->setHeaderData(5, Qt::Horizontal, QVariant(QuestionRole), DataRoleIdRole);
    pModel->setHeaderData(5, Qt::Horizontal, QString("Deine lieblings Farbe?"), PlaceHolderRole);
    pModel->setHeaderData(5, Qt::Horizontal, QVariant(true), EditableRole);
    pModel->setHeaderData(5, Qt::Horizontal, QVariant(QVariant::String), StandardValueRole);
    pModel->setHeaderData(5, Qt::Horizontal, QVariant(QVariant::String), DataTypeRole);
    pModel->setHeaderData(5, Qt::Horizontal, QVariant(false), ColumnVisibleRole);

    pModel->setHeaderData(6, Qt::Horizontal, QString("Antwort"), HeaderNameRole);
    pModel->setHeaderData(6, Qt::Horizontal, QString("answer"), DataRoleNameRole);
    pModel->setHeaderData(6, Qt::Horizontal, QVariant(AnswerRole), DataRoleIdRole);
    pModel->setHeaderData(6, Qt::Horizontal, QString("Grün"), PlaceHolderRole);
    pModel->setHeaderData(6, Qt::Horizontal, QVariant(true), EditableRole);
    pModel->setHeaderData(6, Qt::Horizontal, QVariant(QVariant::String), StandardValueRole);
    pModel->setHeaderData(6, Qt::Horizontal, QVariant(QVariant::String), DataTypeRole);
    pModel->setHeaderData(6, Qt::Horizontal, QVariant(false), ColumnVisibleRole);

    pModel->setHeaderData(7, Qt::Horizontal, QString("Zeichensatz"), HeaderNameRole);
    pModel->setHeaderData(7, Qt::Horizontal, QString("definedcharacter"), DataRoleNameRole);
    pModel->setHeaderData(7, Qt::Horizontal, QVariant(DefinedCharacterRole), DataRoleIdRole);
    pModel->setHeaderData(7, Qt::Horizontal, QString("Zeichen für Passwort"), PlaceHolderRole);
    pModel->setHeaderData(7, Qt::Horizontal, QVariant(true), EditableRole);
    pModel->setHeaderData(7, Qt::Horizontal, QVariant(QString("*[a-Z]*[0-9]")), StandardValueRole);
    pModel->setHeaderData(7, Qt::Horizontal, QVariant(QVariant::String), DataTypeRole);
    pModel->setHeaderData(7, Qt::Horizontal, QVariant(false), ColumnVisibleRole);

    pModel->setHeaderData(8, Qt::Horizontal, QString("Letzte Änderung"), HeaderNameRole);
    pModel->setHeaderData(8, Qt::Horizontal, QString("lastmodify"), DataRoleNameRole);
    pModel->setHeaderData(8, Qt::Horizontal, QVariant(LastModifyRole), DataRoleIdRole);
    pModel->setHeaderData(8, Qt::Horizontal, QString("Datum"), PlaceHolderRole);
    pModel->setHeaderData(8, Qt::Horizontal, QVariant(false), EditableRole);
    pModel->setHeaderData(8, Qt::Horizontal, QVariant(QVariant::DateTime), StandardValueRole);
    pModel->setHeaderData(8, Qt::Horizontal, QVariant(QVariant::DateTime), DataTypeRole);
    pModel->setHeaderData(8, Qt::Horizontal, QVariant(false), ColumnVisibleRole);

    return true;
}
