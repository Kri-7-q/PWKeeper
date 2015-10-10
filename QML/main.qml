import QtQuick 2.3
import QtQuick.Controls 1.2
import Controlers 1.0
import Models 1.0

ApplicationWindow {
    visible: true
    color: "white"
    width: 480
    height: 960
    title: qsTr("Password Kepper")

    TableModel {
        id: tableModel
    }

    ViewControler {
        id: viewControler
    }

    Item {
        anchors.fill: parent

        // Title bar
        // -----------------------------------------------------------
        Rectangle {
            id: titleBar
            width: parent.width
            height: 80
            color: "orange"
            anchors.top: parent.top

            Text {
                font.pixelSize: 24
                text: qsTr("Passwort Keeper")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        // List
        // -----------------------------------------------------------
        AccountList {
            id: accountList
            anchors {
                top: titleBar.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }

        // Dialog to modify an Account
        // -----------------------------------------------------------
        ModifyDialog {
            id: modifyDialog
            anchors {
                top: titleBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }

        // Dialog to show an account
        // -------------------------------------------------------------
        ShowDialog {
            id: showDialog
            width: parent.width
            anchors {
                top: titleBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }

        // List model to get a field name and the key (descriptor) to the value.
        ListModel {
            id: dataInfoModel

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

            // Find editable data and get a list of their model roles.
            function getEditableRoles() {
                var list = []
                for (var i=0; i<count; ++i) {
                    if (get(i).editable) {
                        list.push(get(i).descriptor)
                    }
                }

                return list
            }
        }

        // -----------------------------------------------------------------------
        // States
        states: [
            State {
                // State:   ShowList
                when: viewControler.currentView === ViewControler.AccountList
                PropertyChanges { target: accountList; visible: true }
                PropertyChanges { target: modifyDialog; visible: false }
                PropertyChanges { target: showDialog; visible: false }
            },
            State {
                // State:   ModifyAccount
                when: viewControler.currentView === ViewControler.ModifyAccount
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: modifyDialog; visible: true; }
                PropertyChanges { target: showDialog; visible: false }
            },
            State {
                // State:   ShowAccount
                when: viewControler.currentView === ViewControler.ShowAccount
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: modifyDialog; visible: false; }
                PropertyChanges { target: showDialog; visible: true }
            }
        ]
    } // END Item
} // END ApplicationWindow
