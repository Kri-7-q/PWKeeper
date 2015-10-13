import QtQuick 2.0
import QtQuick.Controls 1.2
import Controllers 1.0
import Models 1.0

Item {

    ListViewController {
        id: listViewController
        model: tableModel
    }

    property alias currentRow: tableView.currentRow

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
        id: tableView
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
        itemDelegate: TableViewItem { fontSize: 14; fontFamily: "Avenir" }
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
                visible: viewController.currentView === ViewController.AccountList
                onClicked: addDialog.initializeAddDialog()
            }
            Button {
                width: 30
                height: 30
                text: qsTr("-")
                style: PushButtonStyle {}
                visible: (viewController.currentView === ViewController.AccountList) && tableView.currentRow >= 0
                onClicked: listViewController.deleteModelRow(tableView.currentRow)
            }
            Button {
                height: 30
                text: qsTr("Show")
                visible: (viewController.currentView === ViewController.AccountList) && tableView.currentRow >= 0
                style: PushButtonStyle {}
                onClicked: viewController.currentView = ViewController.ShowAccount
            }
            Button {
                height: 30
                text: qsTr("Modify")
                style: PushButtonStyle {}
                visible: (viewController.currentView === ViewController.AccountList) && tableView.currentRow >= 0
                onClicked: viewController.currentView = ViewController.ModifyAccount
            }
            Button {
                height: 30
                text: qsTr("Password")
                onClicked: listViewController.copyPasswordToClipboard(tableView.currentRow)
                style: PushButtonStyle {}
                visible: (viewController.currentView === ViewController.AccountList) && tableView.currentRow >= 0
            }
            Button {
                height: 30
                text: qsTr("Undo")
                style: PushButtonStyle {}
                visible: accountList.currentRow >= 0 && (tableModel.modelRowState(accountList.currentRow) !== TableModel.Origin)
            }
            Button {
                height: 30
                text: qsTr("Save")
                style: PushButtonStyle {}
                visible: tableModel.isModified
            }
        } // END - Row (controlBarButtonRow)
    } // END - ControlBar (Rectangle)
}

