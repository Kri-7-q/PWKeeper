import QtQuick 2.3
import QtQuick.Controls 1.2
import Controllers 1.0
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

    ViewController {
        id: viewController
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
            fontFamily: "Avenir"
        }

        // Dialog to show an account
        // -------------------------------------------------------------
        ShowDialog {
            id: showDialog
            anchors {
                top: titleBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            fontFamily: "Avenir"
        }

        // Dialog to insert a new Account
        // -------------------------------------------------------------
        AddDialog {
            id: addDialog
            anchors {
                top: titleBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            fontFamily: "Avenir"
        }

        // List model to get a field name and the key (descriptor) to the value.
        ListModel {
            id: dataInfoModel

            ListElement {
                name: "Id"
                roleName: "id"
                placeHolder: "Primary Key"
                editable: false
            }
            ListElement {
                name: "Provider"
                roleName: "provider"
                placeHolder: "Provider name"
                editable: true
            }
            ListElement {
                name: "Benutzername"
                roleName: "username"
                placeHolder: "Horst Krampf"
                editable: true
            }
            ListElement {
                name: "Passwort"
                roleName: "password"
                placeHolder: "Secret Key"
                editable: true
            }
            ListElement {
                name: "Length"
                roleName: "passwordlength"
                placeHolder: "12"
                editable: true
            }
            ListElement {
                name: "Frage"
                roleName: "question"
                placeHolder: "What is that?"
                editable: true
            }
            ListElement {
                name: "Antwort"
                roleName: "answer"
                placeHolder: "I don't know."
                editable: true
            }
            ListElement {
                name: "Zeichensatz"
                roleName: "definedcharacter"
                placeHolder: "*[a-Z]*[0-9]"
                editable: true
            }
            ListElement {
                name: "Letzte Änderung"
                roleName: "lastmodify"
                placeHolder: "Date of last changes."
                editable: false
            }

            // Find editable data and get a list of their model roles.
            function getEditableRoles() {
                var list = []
                for (var i=0; i<count; ++i) {
                    if (get(i).editable) {
                        list.push(get(i).roleName)
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
                when: viewController.currentView === ViewController.AccountList
                PropertyChanges { target: accountList; visible: true }
                PropertyChanges { target: modifyDialog; visible: false }
                PropertyChanges { target: showDialog; visible: false }
                PropertyChanges { target: addDialog; visible: false }
            },
            State {
                // State:   ModifyAccount
                when: viewController.currentView === ViewController.ModifyAccount
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: modifyDialog; visible: true; }
                PropertyChanges { target: showDialog; visible: false }
                PropertyChanges { target: addDialog; visible: false }
            },
            State {
                // State:   ShowAccount
                when: viewController.currentView === ViewController.ShowAccount
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: modifyDialog; visible: false; }
                PropertyChanges { target: showDialog; visible: true }
                PropertyChanges { target: addDialog; visible: false }
            },
            State {
                // State: Add new Account
                when: viewController.currentView === ViewController.NewAccount
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: modifyDialog; visible: false }
                PropertyChanges { target: showDialog; visible: false }
                PropertyChanges { target: addDialog; visible: true }
            }

        ]
    } // END Item
} // END ApplicationWindow
