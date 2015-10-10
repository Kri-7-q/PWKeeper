import QtQuick 2.0
import QtQuick.Controls 1.2
import Controlers 1.0

Item {
    id:root

    ModifyControler {
        id: modifyControler
        model: tableModel
    }

    onVisibleChanged: {
        if (visible === true) {
            model = modifyControler.modelRowEntry(accountList.currentRow)
        }
    }

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
            model: dataInfoModel

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
                onClicked: viewControler.currentView = ViewControler.AccountList
            }
            Button {
                text: qsTr("Generate Password")
                height: 30
                style: PushButtonStyle {}
                onClicked: {
                    var password = modifyControler.generatePassword(modifiedData())
                    var tempModel = model
                    tempModel["password"] = password
                    model = tempModel
                }
            }
            Button {
                text: qsTr(" OK ")
                height: 30
                style: PushButtonStyle {}
                onClicked: {
                    modifyControler.setModifiedData(accountList.currentRow, modifiedData(), dataInfoModel.getEditableRoles())
                    viewControler.currentView = ViewControler.AccountList
                }
            }
        }
    }

    // Get modified data from input fields.
    function modifiedData() {
        for (var i=0; i<dataInfoModel.count; ++i) {
            if (dataInfoModel.get(i).editable !== true) {
                continue
            }
            var modelKey = dataInfoModel.get(i).descriptor
            var value = textFieldRepeater.itemAt(i).text
            root.model[modelKey] = value
        }

        return root.model
    }
}

