import QtQuick 2.0
import QtQuick.Controls 1.2
import Controllers 1.0
import Models 1.0

Item {
    id: root

    // Controler to this view
    AddController {
        id: addController
        model: tableViewModel
    }

    property int descriptionWidth: width / 3
    property string fontFamily: "Arial"
    property int fontSize: 16
    property int fontWeight: Font.Normal
    property alias model: textFieldRepeater.model

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
            model: 0

            Row {
                id: entryRow
                spacing: 10

                property alias text: entryTextField.text

                // Value description field
                Text {
                    width: descriptionWidth
                    text: tableViewModel.headerData(index, "headerName")
                    visible: tableViewModel.headerData(index, "editable")
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: Font.Bold
                    }
                }

                // Value input field
                TextField {
                    id: entryTextField
                    width: root.width - entryRow.spacing - root.descriptionWidth - entryColumn.anchors.margins * 2
                    text: tableViewModel.headerData(index, "standardValue")
                    placeholderText: tableViewModel.headerData(index, "placeHolder")
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: root.fontWeight
                    }
                    visible: tableViewModel.headerData(index, "editable")
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
                    addController.insertNewData(insertedData())
                    viewController.currentView = ViewController.AccountList
                }
            }
        }
    }

    // Get inserted data from input fields.
    function insertedData() {
        var data = {}
        for (var i=0; i<tableViewModel.columnCount(); ++i) {
            if (!tableViewModel.headerData(i, "editable")) {
                continue
            }
            var modelKey = tableViewModel.headerData(i, "roleName")
            var value = textFieldRepeater.itemAt(i).text
            data[modelKey] = value
        }

        return data
    }

    // Get a new password and set it into text field.
    function generatePassword() {
        var password = addController.generatePassword(insertedData())
        var index = 0;
        for ( ; index<tableViewModel.columnCount(); ++index) {
            var role = tableViewModel.headerData(index, "roleName")
            if (role === "password") {
                break;
            }
        }
        textFieldRepeater.itemAt(index).text = password
    }
}

