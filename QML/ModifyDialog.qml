import QtQuick 2.0
import QtQuick.Controls 1.2
import Controllers 1.0

Item {
    id:root

    // Controler of this view
    ModifyController {
        id: modifyController
        model: tableViewModel
    }

    onVisibleChanged: {
        if (visible === true) {
            modelRow = accountList.currentRow
            errorText.text = ""
            textFieldRepeater.model = tableViewModel.columnCount()
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
            model: 0

            Row {
                id: entryRow
                spacing: 10

                property alias text: entryTextField.text

                // Value description field
                Text {
                    width: root.descriptionWidth
                    text: modifyController.modelHeaderText(index)
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
                    text: modifyController.modelData(index, modelRow)
                    placeholderText: modifyController.modelHeaderPlaceHolder(index);
                    visible: modifyController.isColumnEditable(index);
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: root.fontWeigth
                    }
                }
                // Shows not editable values (is visible if value is not editable)
                Text {
                    id: entryText
                    text: modifyController.modelData(index, modelRow)
                    visible: !modifyController.isColumnEditable(index)
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
        for (var i=0; i<tableViewModel.columnCount(); ++i) {
            if (modifyController.isColumnEditable(i) !== true) {
                continue
            }
            var modelKey = modifyController.modelHeaderRoleName(i)
            var value = textFieldRepeater.itemAt(i).text
            data[modelKey] = value
        }

        return data
    }

    // Generate a new password and set it into text field.
    function generatePassword() {
        var password = modifyController.generatePassword(modifiedData())
        var index = modifyController.modelsPasswordSection()
        textFieldRepeater.itemAt(index).text = password
    }
}

