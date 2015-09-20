import QtQuick 2.0
import QtQuick.Controls 1.2
import Controlers 1.0

Item {
    // ---------------------------------------------------
    // Search bar
    SearchBar {
        id: searchBar
        anchors.top: parent.top
        width: parent.width
        height: 80
    }

    // ---------------------------------------------------
    // Table view
    TableView {
        id: accountList
        width: parent.width
        model: tableModel
        anchors {
            bottom: controlBar.top
            top: searchBar.bottom
        }
        alternatingRowColors: false

        TableViewColumn { role: "provider"; title: "Provider"; width: 150 }
        TableViewColumn { role: "username"; title: "Benutzername"; width: 200 }

        headerDelegate: TableViewHeader { fontSize: 14; borderWidth: 0 }
        itemDelegate: TableViewItem { fontSize: 14 }
        rowDelegate: TableViewRow { height: 20 }

        Keys.onReturnPressed: controler.copyPasswordToClipboard(currentRow)
    }

    // ---------------------------------------------------
    // Button bar
    Rectangle {
        id: controlBar
        width: parent.width
        height: 80
        color: "lightgray"
        anchors.bottom: parent.bottom


        Row {
            id: controlBarButtonRow
            spacing: 20
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 20
            }

            Button {
                width: 30
                height: 30
                text: qsTr("+")
                style: PushButtonStyle {}
                visible: controler.currentView === PWKeeperControler.AccountList
            }
            Button {
                width: 30
                height: 30
                text: qsTr("-")
                style: PushButtonStyle {}
                visible: controler.currentView === PWKeeperControler.AccountList
            }
            Button {
                height: 30
                text: qsTr("Show")
                visible: (controler.currentView === PWKeeperControler.AccountList) && accountList.currentRow > -1
                style: PushButtonStyle {}
                onClicked: controler.currentView = PWKeeperControler.ShowAccount
            }
            Button {
                height: 30
                text: qsTr("Modify")
                style: PushButtonStyle {}
                visible: (controler.currentView === PWKeeperControler.AccountList) && accountList.currentRow > -1
                onClicked: controler.currentView = PWKeeperControler.ModifyAccount
            }
            Button {
                height: 30
                text: qsTr("Password")
                onClicked: controler.copyPasswordToClipboard(accountList.currentRow)
                style: PushButtonStyle {}
                visible: (controler.currentView === PWKeeperControler.AccountList) && accountList.currentRow > -1
            }
            Button {
                height: 30
                text: qsTr("Save changes")
                style: PushButtonStyle {}
                visible: controler.dataModified
            }
        } // END - Row (controlBarButtonRow)
    } // END - ControlBar (Rectangle)
}
