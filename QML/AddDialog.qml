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
    property var model: {"id":"", "provider":"", "username":"", "password":"", "question":"",
                         "answer":"", "definedcharacter":"*[A-Z]*[a-z]*[0-9]*{()<>!$%&/=?+*#-.,;:_}",
                         "lastmodify":"", "passwordlength":"12"}
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
                onClicked: {
                    model = insertedData()
                    var password = addController.generatePassword(model)
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
                    addController.insertNewData(insertedData(), dataInfoModel.getEditableRoles());
                    viewController.currentView = ViewController.AccountList
                }
            }
        }
    }

    // Get inserted data from input fields.
    function insertedData() {
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

}

