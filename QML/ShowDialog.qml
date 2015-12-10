import QtQuick 2.0
import QtQuick.Controls 1.2
import Controllers 1.0

Item {
    id: root

    // Controler of this view
    DialogController {
        id: showController
        model: tableViewModel
    }

    // If this view is switched to visible then it sets the current selected row of ListView.
    onVisibleChanged: {
        if (visible === true) {
            modelRow = accountList.currentRow
            inputFieldRepeater.model = tableViewModel.columnCount()
        }
    }

    property int descriptionWidth: width / 3
    property int modelRow: 0
    property string fontFamily: "Arial"
    property int fontSize: 16
    property int fontWeigth: Font.Normal


    // ----------------------------------------------------------
    // Show one Account
    Column {
        spacing: 10
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 20
        }

        // Shows as many fields as TableModel has columns. (each value of an Account object)
        Repeater {
            id: inputFieldRepeater
            model: 0

            Row {
                spacing: 10

                // Values description field
                Text {
                    width: root.descriptionWidth
                    text: tableViewModel.headerData(index, "headerName")
                    font {
                        bold: true
                        family: root.fontFamily
                        pixelSize: root.fontSize
                    }
                }
                // Value field
                Text {
                    text: showController.modelData(index, modelRow)
                    font {
                        family: root.fontFamily
                        pixelSize: root.fontSize
                        weight: root.fontWeigth
                    }
                }
            }
        }
    } // End - Column

    // -------------------------------------------------------------------
    // Button bar
    Rectangle {
        width: parent.width
        height: 80
        anchors.bottom: parent.bottom
        color: "lightgray"

        Row {
            spacing: 20
            anchors {
                leftMargin: 20
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            Button {
                height: 30
                text: qsTr(" Back ")
                style: PushButtonStyle {}
                onClicked: viewController.currentView = ViewController.AccountList
            }
        }
    }
}
