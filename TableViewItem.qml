import QtQuick 2.0

Rectangle {
    color: "transparent"

    property color normalTextColor: "black"
    property color highlightTextColor: "blue"
    property alias fontFamily: itemText.font.family
    property alias fontSize: itemText.font.pixelSize
    property alias fontWeight: itemText.font.weight
    property int textMargin: 3

    Text {
        id: itemText
        color: styleData.selected ? parent.highlightTextColor : parent.normalTextColor
        text: styleData.value
        elide: styleData.elideMode
        horizontalAlignment: styleData.textAlignment | Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: parent.textMargin
        font {
            family: "Helvetica"
            pixelSize: 12
            weight: Font.Normal
        }
    }
}
