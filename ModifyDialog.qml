import QtQuick 2.0
import QtQuick.Controls 1.2
import Controlers 1.0

Item {
    id:root

    property int descriptionWidth: width / 3
    property var model: {"id":"none", "provider":"none", "username":"none", "password":"none", "question":"none",
                         "answer":"none", "definedcharacter":"none", "lastmodify":"none", "passwordlength":"none"}

    // ---------------------------------------------------------------
    // TextField controls to modify an account
    Column {
        id: entryColumn
        spacing: 10
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 20
        }

        Repeater {
            id: textFieldRepeater
            model: translationModel

            Row {
                id: entryRow
                spacing: 10

                property alias text: entryTextField.text

                Text {
                    width: root.descriptionWidth
                    text: name
                    font.bold: true
                    font.family: "Arial"
                    font.pixelSize: 16
                }
                TextField {
                    id: entryTextField
                    width: root.width - entryRow.spacing - root.descriptionWidth - entryColumn.anchors.margins * 2
                    text: root.model[descriptor]
                    font.family: "Arial"
                    font.pixelSize: 16
                    visible: editable
                }
                Text {
                    id: entryText
                    text: root.model[descriptor]
                    font.family: "Arial"
                    font.pixelSize: 16
                    visible: !editable
                }
            }
        }
    } // End - Column

    // ------------------------------------------------------------------------
    // Button bar
    Rectangle {
        height: 80
        width: parent.width
        anchors.bottom: parent.bottom
        color: "lightgray"

        Row {
            spacing: 20
            anchors {
                left: parent.left
                leftMargin: 20
                verticalCenter: parent.verticalCenter
            }

            Button {
                text: qsTr("Back")
                height: 30
                style: PushButtonStyle {}
                onClicked: controler.currentView = PWKeeperControler.AccountList
            }
            Button {
                text: qsTr("OK")
                height: 30
                style: PushButtonStyle {}
                onClicked: controler.setModifiedData(modifiedData())
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
            editable: false
        }
        ListElement {
            name: "Provider"
            descriptor: "provider"
            editable: true
        }
        ListElement {
            name: "Benutzername"
            descriptor: "username"
            editable: true
        }
        ListElement {
            name: "Passwort"
            descriptor: "password"
            editable: true
        }
        ListElement {
            name: "Length"
            descriptor: "passwordlength"
            editable: true
        }
        ListElement {
            name: "Frage"
            descriptor: "question"
            editable: true
        }
        ListElement {
            name: "Antwort"
            descriptor: "answer"
            editable: true
        }
        ListElement {
            name: "Zeichensatz"
            descriptor: "definedcharacter"
            editable: true
        }
        ListElement {
            name: "Letzte Änderung"
            descriptor: "lastmodify"
            editable: false
        }
    }
}

