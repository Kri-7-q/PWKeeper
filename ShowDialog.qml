import QtQuick 2.0
import QtQuick.Controls 1.2
import Controlers 1.0

Item {
    id: root

    property int descriptionWidth: width / 3
    property var model: {"id":"none", "provider":"none", "username":"none", "password":"none", "question":"none",
                         "answer":"none", "definedcharacter":"none", "lastmodify":"none", "passwordlength":"none"}


    // ----------------------------------------------------------
    // Show one Account
    Column {
        spacing: 10
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 20
        }

        Repeater {
            model: translationModel

            Row {
                spacing: 10

                Text {
                    width: root.descriptionWidth
                    text: name
                    font.bold: true
                    font.family: "Arial"
                    font.pixelSize: 16
                }
                Text {
                    text: root.model[descriptor]
                    font.family: "Arial"
                    font.pixelSize: 16
                }
            }
        }
    } // End - Column

    // -------------------------------------------------------------------
    // Button bar
    Rectangle {
        width: parent.width
        height: 80
        anchors.bottom: parent.bottom
        color: "lightgray"

        Row {
            spacing: 20
            anchors {
                leftMargin: 20
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            Button {
                height: 30
                text: qsTr("Back")
                style: PushButtonStyle {}
                onClicked: controler.currentView = PWKeeperControler.AccountList
            }
        }
    }

    // -----------------------------------------------------------------------
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
            name: "Length"
            descriptor: "passwordlength"
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
