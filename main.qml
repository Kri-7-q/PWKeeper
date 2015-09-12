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

    PWKeeperControler {
        id: controler
        model: tableModel
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

        // Search bar
        // -----------------------------------------------------------
        SearchBar {
            id: searchBar
            width: parent.width
            height: 80
            controlHigh: 30
            anchors.top: titleBar.bottom
        }

        // List
        // -----------------------------------------------------------
        TableView {
            id: accountList
            width: parent.width
            model: tableModel
            anchors.top: searchBar.bottom
            anchors.bottom: controlBar.top
            alternatingRowColors: false

            TableViewColumn { role: "provider"; title: "Provider"; width: 150 }
            TableViewColumn { role: "username"; title: "Benutzername"; width: 200 }

            headerDelegate: TableViewHeader { fontSize: 14; borderWidth: 0 }
            itemDelegate: TableViewItem { fontSize: 14 }
            rowDelegate: TableViewRow { height: 20 }

            Keys.onReturnPressed: controler.copyPasswordToClipboard(currentRow)
        }

        // Dialog to modify or show an Account
        // -----------------------------------------------------------
        AccountDialog {
            id: accountDialog
            width: parent.width
            anchors {
                top: titleBar.bottom
                bottom: controlBar.top
                left: parent.left
                right: parent.right
                margins: 20
            }
            onVisibleChanged: {
                if (visible === true) {
                    model = controler.modelRowEntry(accountList.currentRow)
                }
            }
        }

        // Control bar
        // -----------------------------------------------------------
        ButtonBar {
            id: controlBar
            anchors.bottom: parent.bottom
        }

        // -----------------------------------------------------------------------
        // States
        states: [
            State {
                // State:   ShowList
                when: controler.currentView === PWKeeperControler.AccountList
                PropertyChanges { target: searchBar; visible: true }
                PropertyChanges { target: accountList; visible: true }
                PropertyChanges { target: accountDialog; visible: false }
            },
            State {
                // State:   ModifyAccount
                when: controler.currentView === PWKeeperControler.ModifyAccount
                PropertyChanges { target: searchBar; visible: false }
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: accountDialog; visible: true; editable: true }
            },
            State {
                // State:   ShowAccount
                when: controler.currentView === PWKeeperControler.ShowAccount
                PropertyChanges { target: searchBar; visible: false }
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: accountDialog; visible: true; editable: false }
            }
        ]
    } // END Item
} // END ApplicationWindow
