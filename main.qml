import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    visible: true
    color: "white"
    width: 480
    height: 960
    title: qsTr("Password Kepper")

    // Title bar
    // -----------------------------------------------------------
    Rectangle {
        id: titleBar
        width: parent.width
        height: 80
        color: "lightblue"
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
    Rectangle {
        id: searchBar
        width: parent.width
        height: 80
        color: "lightgray"
        anchors.top: titleBar.bottom

        Row {
            spacing: 20
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }

            ComboBox {
                id: searchType
                model: [qsTr("Provider"), qsTr("Username")]
            }
            SearchField {
                id: searchText
                width: 150
                onClicked: controler.findValuesFor(searchType.currentText, text)
                Keys.onReturnPressed: controler.findValuesFor(searchType.currentText, text)
            }
            Button {
                text: qsTr("Show all")
                onClicked: controler.showAllData()
            }
        }
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

    // Control bar
    // -----------------------------------------------------------
    Rectangle {
        id: controlBar
        width: parent.width
        height: 80
        color: "orange"
        anchors.bottom: parent.bottom

        Row {
            spacing: 20
            anchors.centerIn: parent

            Button {
                text: qsTr("+")
            }
            Button {
                text: qsTr("-")
            }
            Button {
                text: qsTr("Modify")
                enabled: accountList.currentRow >= 0
            }
            Button {
                text: qsTr("Save changes")
                enabled: false
            }
            Button {
                text: qsTr("Password")
                enabled: accountList.currentRow >= 0
                onClicked: controler.copyPasswordToClipboard(accountList.currentRow)
            }
        }
    }
}
