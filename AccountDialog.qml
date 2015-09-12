import QtQuick 2.0
import QtQuick.Controls 1.2

Column {
    id: entryViewColumn
    spacing: 10

    property bool editable: false
    property int descriptionWidth: width / 3
    property var model: {"id":"none", "provider":"none", "username":"none", "password":"none", "question":"none",
                         "answer":"none", "definedcharacter":"none", "lastmodify":"none"}

    Repeater {
        id: textFieldRepeater
        model: translationModel

        Row {
            spacing: 10

            property alias text: entryTextField.text

            Text {
                width: entryViewColumn.descriptionWidth
                text: name
                font.bold: true
                font.family: "Arial"
                font.pixelSize: 16
            }
            Text {
                text: entryViewColumn.model[descriptor]
                font.family: "Arial"
                font.pixelSize: 16
                visible: !entryViewColumn.editable || index === 0 || index === 7
            }
            TextField {
                id: entryTextField
                width: entryViewColumn.width - entryViewColumn.spacing - entryViewColumn.descriptionWidth
                text: entryViewColumn.model[descriptor]
                font.family: "Arial"
                font.pixelSize: 16
                visible: entryViewColumn.editable && index !== 0 && index !== 7
            }
        }
    }

    // Get modified data from input fields.
    function modifiedData() {
        var account = { "id":textFieldRepeater.itemAt(0).text,
                        "provider":textFieldRepeater.itemAt(1).text,
                        "username":textFieldRepeater.itemAt(2).text,
                        "password":textFieldRepeater.itemAt(3).text,
                        "question":textFieldRepeater.itemAt(4).text,
                        "answer":textFieldRepeater.itemAt(5).text,
                        "definedcharacter":textFieldRepeater.itemAt(6).text,
                        "lastmodify":textFieldRepeater.itemAt(7).text }

        return account
    }

    // List model to get a field name and the key (descriptor) to the value.
    ListModel {
        id: translationModel

        ListElement {
            name: "Id"
            descriptor: "id"
        }
        ListElement {
            name: "Provider"
            descriptor: "provider"
        }
        ListElement {
            name: "Benutzername"
            descriptor: "username"
        }
        ListElement {
            name: "Passwort"
            descriptor: "password"
        }
        ListElement {
            name: "Frage"
            descriptor: "question"
        }
        ListElement {
            name: "Antwort"
            descriptor: "answer"
        }
        ListElement {
            name: "Zeichensatz"
            descriptor: "definedcharacter"
        }
        ListElement {
            name: "Letzte Änderung"
            descriptor: "lastmodify"
        }
    }
}
