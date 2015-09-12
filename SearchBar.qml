import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    id: searchBarControl
    color: "lightgray"

    property int controlHigh: 25

    Row {
        spacing: 20
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        ComboBox {
            id: searchType
            height: searchBarControl.controlHigh
            model: [qsTr("Provider"), qsTr("Username")]
        }
        SearchField {
            id: searchText
            width: 150
            height: searchBarControl.controlHigh
            onClicked: controler.findValuesFor(searchType.currentText, text)
            Keys.onReturnPressed: controler.findValuesFor(searchType.currentText, text)
        }
        Button {
            height: searchBarControl.controlHigh
            text: qsTr("Show all")
            onClicked: controler.showAllData()
            style: PushButtonStyle { color: "steelblue"; highlightColor: "lightblue" }
        }
    }
}
