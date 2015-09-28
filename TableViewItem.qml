import QtQuick 2.0
import Models 1.0

Rectangle {
    color: "transparent"

    property color normalTextColor: "black"
    property color highlightTextColor: "blue"
    property color modifiedtextColor: "orange"
    property color deletedTextColor: "red"
    property alias fontFamily: itemText.font.family
    property alias fontSize: itemText.font.pixelSize
    property alias fontWeight: itemText.font.weight
    property int textMargin: 3

    Text {
        id: itemText
        color: textColor()
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
        onColorChanged: {
            font.strikeout = (tableModel.modelRowState(styleData.row) === TableModel.Deleted)
            font.italic = (tableModel.modelRowState(styleData.row) === TableModel.Modified)
        }
    }

    // Text color desition.
    function textColor() {
        var color = "yellow"
        var modelRowState = tableModel.modelRowState(styleData.row)

        if (modelRowState === TableModel.Origin) {
            color = (styleData.selected) ? highlightTextColor : normalTextColor
        }
        else if (modelRowState === TableModel.Modified) {
            color = modifiedtextColor
        }
        else if (modelRowState === TableModel.Deleted) {
            color = deletedTextColor
        }

        return color
    }
}
