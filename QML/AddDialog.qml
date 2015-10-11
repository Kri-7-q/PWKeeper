import QtQuick 2.0
import QtQuick.Controls 1.2
import Controllers 1.0

Item {
    id: root

    AddController {
        id: addController
        model: tableModel
    }

    property int descriptionWidth: width / 3
    property var model: addController.dialogModel
    property string fontFamily: "Arial"
    property int fontSize: 16
    property int fontWeight: Font.Normal

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
                    width: descriptionWidth
                    text: name
                    visible: editable
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: Font.Bold
                    }
                }

                TextField {
                    id: entryTextField
                    width: root.width - entryRow.spacing - root.descriptionWidth - entryColumn.anchors.margins * 2
                    text: root.model[roleName]
                    placeholderText: placeHolder
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: root.fontWeight
                    }
                    visible: editable
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
                    addController.insertNewData(insertedData());
                    viewController.currentView = ViewController.AccountList
                    model = addController.dialogModel
                }
            }
        }
    }

    // Get inserted data from input fields.
    function insertedData() {
        for (var i=0; i<dataInfoModel.count; ++i) {
            if (!dataInfoModel.get(i).editable) {
                continue
            }
            var modelKey = dataInfoModel.get(i).roleName
            var value = textFieldRepeater.itemAt(i).text
            root.model[modelKey] = value
        }

        return root.model
    }

    // Get a new password and set it into text field.
    function generatePassword() {
        var password = addController.generatePassword(insertedData())
        var index = 0;
        for ( ; index<dataInfoModel.count; ++index) {
            var role = dataInfoModel.get(index).roleName
            if (role === "password") {
                break;
            }
        }
        textFieldRepeater.itemAt(index).text = password
    }
}

