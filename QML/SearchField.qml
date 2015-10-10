import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    width: 150
    height: 25
    color: "white"
    border.color: "black"
    border.width: 1

    property alias text: searchText.text
    property int innerWidth: width - border.width * 2
    property int innerHeight: height - border.width * 2
    signal clicked()

    TextField {
        id: searchText
        width: parent.innerWidth - parent.innerHeight
        height: parent.innerHeight
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: parent.border.width
    }
    Rectangle {
        id: searchButton
        width: parent.innerHeight
        height: parent.innerHeight
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: searchText.right
        color: "lightgray"
        Image {
            width: parent.width - 2
            height: parent.height - 2
            anchors.centerIn: parent
            source: "searchBar/lupe.png"
        }
    }
    MouseArea {
        anchors.fill: searchButton
        onClicked: parent.clicked()
    }
}

