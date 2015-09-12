import QtQuick 2.0
import QtQuick.Controls 1.2
import Controlers 1.0

Rectangle {
    width: parent.width
    height: 80
    color: "lightgray"

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
            onClicked: controler.currentView = PWKeeperControler.ModifyAccount
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
        Button {
            id: controlBarBackButton
            height: 30
            text: qsTr("Back")
            style: PushButtonStyle {}
            onClicked: controler.currentView = PWKeeperControler.AccountList
            visible: controler.currentView !== PWKeeperControler.AccountList
        }
        Button {
            height: 30
            text: qsTr("OK")
            style: PushButtonStyle {}
            visible: controler.currentView === PWKeeperControler.ModifyAccount
            onClicked: controler.setModifiedData(accountDialog.modifiedData())
        }
    } // END - Row (controlBarButtonRow)
} // END - ControlBar (Rectangle)
