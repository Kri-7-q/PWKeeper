import QtQuick 2.0
import QtQuick.Controls 1.2
import Controllers 1.0

Item {
    id:root

    ModifyController {
        id: modifyController
        model: tableModel
    }

    onVisibleChanged: {
        if (visible === true) {
            modelRow = accountList.currentRow
        }
    }

    property int descriptionWidth: width / 3
    property int modelRow: 0
    property string fontFamily: "Arial"
    property int fontSize: 16
    property int fontWeigth: Font.Normal

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
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        bold: true
                    }
                }
                TextField {
                    id: entryTextField
                    width: root.width - entryRow.spacing - root.descriptionWidth - entryColumn.anchors.margins * 2
                    text: tableModel.data(modelRow, roleName)
                    placeholderText: placeHolder
                    visible: editable
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: root.fontWeigth
                    }
                }
                Text {
                    id: entryText
                    text: tableModel.data(modelRow, roleName)
                    visible: !editable
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: root.fontWeigth
                    }
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
                onClicked: viewController.currentView = ViewController.AccountList
            }
            Button {
                text: qsTr("Generate Password")
                height: 30
                style: PushButtonStyle {}
                onClicked: generatePassword()
            }
            Button {
                text: qsTr(" OK ")
                height: 30
                style: PushButtonStyle {}
                onClicked: {
                    modifyController.setModifiedData(accountList.currentRow, modifiedData(), dataInfoModel.getEditableRoles())
                    viewController.currentView = ViewController.AccountList
                }
            }
        }
    }

    // Get modified data from input fields.
    function modifiedData() {
        var data = {}
        for (var i=0; i<dataInfoModel.count; ++i) {
            if (dataInfoModel.get(i).editable !== true) {
                continue
            }
            var modelKey = dataInfoModel.get(i).roleName
            var value = textFieldRepeater.itemAt(i).text
            data[modelKey] = value
        }

        return data
    }

    // Generate a new password and set it into text field.
    function generatePassword() {
        var password = modifyController.generatePassword(modifiedData())
        var index = 0
        for ( ; index<dataInfoModel.count; ++index) {
            var role = dataInfoModel.get(index).roleName
            if (role === "password") {
                break
            }
        }
        textFieldRepeater.itemAt(index).text = password
    }
}

