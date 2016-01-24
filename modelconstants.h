#ifndef MODELCONSTANTS
#define MODELCONSTANTS

#include <Qt>


// Below ModelRoles should be used in 'getModelHeader()'
// PrimaryKey, Provider, Username, Password, PasswordLength, DefinedCharacter MUST be present.
// The others are optional.
enum ModelDataRole { PrimaryKeyRole = Qt::UserRole, ProviderRole, UsernameRole, PasswordRole, PasswordLengthRole,
                 DefinedCharacterRole, QuestionRole, AnswerRole, LastModifyRole, StateRole};


/*
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
*/
enum ModelHeaderRole { HeaderNameRole = Qt::UserRole + 30, DataRoleNameRole, DataRoleIdRole, PlaceHolderRole,
                       EditableRole, StandardValueRole, DataTypeRole, ColumnVisibleRole };

#endif // MODELCONSTANTS

