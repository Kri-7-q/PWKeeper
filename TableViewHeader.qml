import QtQuick 2.0

Item {
    id: headerItem
    height: 25

    property alias color: headerItemRect.color
    property color borderColor: "black"
    property int borderWidth: 1
    property alias fontFamily: headerText.font.family
    property alias fontSize: headerText.font.pixelSize
    property alias fontWeight: headerText.font.weight
    property alias textColor: headerText.color
    property int textMargins: 3

    Rectangle {
        id: headerItemBorder
        color: "transparent"
        border.color: parent.borderColor
        border.width: parent.borderWidth
        anchors.fill: parent

        Rectangle {
            id: headerItemRect
            color: "lightgreen"
            anchors {
                fill: parent
                leftMargin: 0
                rightMargin: parent.border.width
                topMargin: (parent.border.width > 0) ? parent.border.width - 1 : parent.border.width
                bottomMargin: parent.border.width
            }

            Text {
                id: headerText
                text: styleData.value
                horizontalAlignment: styleData.textAlignment
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    margins: headerItem.textMargins
                }
                font {
                    family: "Helvetica"
                    pixelSize: 12
                    weight: Font.Bold
                }
            }
        }
    }
}
