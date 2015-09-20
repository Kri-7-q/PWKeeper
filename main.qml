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
            onVisibleChanged: {
                if (visible === true) {
                    model = controler.modelRowEntry(accountList.currentRow)
                }
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
            onVisibleChanged: {
                if (visible === true) {
                    model = controler.modelRowEntry(accountList.currentRow)
                }
            }
        }

        // -----------------------------------------------------------------------
        // States
        states: [
            State {
                // State:   ShowList
                when: controler.currentView === PWKeeperControler.AccountList
                PropertyChanges { target: accountList; visible: true }
                PropertyChanges { target: modifyDialog; visible: false }
                PropertyChanges { target: showDialog; visible: false }
            },
            State {
                // State:   ModifyAccount
                when: controler.currentView === PWKeeperControler.ModifyAccount
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: modifyDialog; visible: true; }
                PropertyChanges { target: showDialog; visible: false }
            },
            State {
                // State:   ShowAccount
                when: controler.currentView === PWKeeperControler.ShowAccount
                PropertyChanges { target: accountList; visible: false }
                PropertyChanges { target: modifyDialog; visible: false; }
                PropertyChanges { target: showDialog; visible: true }
            }
        ]
    } // END Item
} // END ApplicationWindow
