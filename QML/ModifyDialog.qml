import QtQuick 2.0
import QtQuick.Controls 1.2
import Controllers 1.0

Item {
    id:root

    // Controler of this view
    ModifyController {
        id: modifyController
        model: tableModel
    }

    onVisibleChanged: {
        if (visible === true) {
            modelRow = accountList.currentRow
            errorText.text = ""
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

        // Shows as many fields as an Account object has. (all columns in the model)
        Repeater {
            id: textFieldRepeater
            model: tableModel.columnCount()

            Row {
                id: entryRow
                spacing: 10

                property alias text: entryTextField.text

                // Value description field
                Text {
                    width: root.descriptionWidth
                    text: tableModel.headerData(index, "headerName")
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        bold: true
                    }
                }
                // Value input field (is visible if value is editable)
                TextField {
                    id: entryTextField
                    width: root.width - entryRow.spacing - root.descriptionWidth - entryColumn.anchors.margins * 2
                    text: {
                        var roleName = tableModel.headerData(index, "roleName")
                        tableModel.data(modelRow, roleName)
                    }
                    placeholderText: tableModel.headerData(index, "placeHolder")
                    visible: tableModel.headerData(index, "editable")
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: root.fontWeigth
                    }
                }
                // Shows not editable values (is visible if value is not editable)
                Text {
                    id: entryText
                    text: {
                        var roleName = tableModel.headerData(index, "roleName")
                        tableModel.data(modelRow, roleName)
                    }
                    visible: !tableModel.headerData(index, "editable")
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
    // Error messages
    Text {
        id: errorText
        color: "red"
        width: parent.width
        height: 30
        anchors {
            top: entryColumn.bottom
            left: parent.left
            margins: 20
        }

        function setErrorMsg(message) {
            text = message
        }

        Component.onCompleted: modifyController.errorMessage.connect(setErrorMsg)
    }

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
                    modifyController.setModifiedData(accountList.currentRow, modifiedData())
                    viewController.currentView = ViewController.AccountList
                }
            }
        }
    }

    // Get modified data from input fields.
    function modifiedData() {
        var data = {}
        for (var i=0; i<tableModel.columnCount(); ++i) {
            if (tableModel.headerData(i, "editable") !== true) {
                continue
            }
            var modelKey = tableModel.headerData(i, "roleName")
            var value = textFieldRepeater.itemAt(i).text
            data[modelKey] = value
        }

        return data
    }

    // Generate a new password and set it into text field.
    function generatePassword() {
        var password = modifyController.generatePassword(modifiedData())
        var index = 0
        for ( ; index<tableModel.columnCount(); ++index) {
            var role = tableModel.headerData(index, "roleName")
            if (role === "password") {
                break
            }
        }
        textFieldRepeater.itemAt(index).text = password
    }
}

