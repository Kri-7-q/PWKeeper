/**
  * TableView item delegate
  * --------------------------
  * Draws a TableView cell. If shows the text of a cell with different colors.
  * It has colors for a selected Item, deleted Item, modified Item and a new Item.
  * In addition to the colors it shows deleted text strikeout and modified text
  * is shown in italic.
  * To update the color and font if catches the models 'dataChanged()' signal.
  *
  * Public properties
  * --------------------
  * - normalTextColor
  * - highlightTextColor
  * - modifiedTextColor
  * - deletedTextColor
  * - newItemTextColor
  * - fontFamily
  * - fontSize
  * - fontWeight
  * - textMargin
  */


import QtQuick 2.0
import Models 1.0


Rectangle {
    color: "transparent"

    property color normalTextColor: "black"
    property color highlightTextColor: "blue"
    property color modifiedTextColor: "orange"
    property color deletedTextColor: "red"
    property color newItemTextColor: "green"
    property alias fontFamily: itemText.font.family
    property alias fontSize: itemText.font.pixelSize
    property alias fontWeight: itemText.font.weight
    property int textMargin: 3
    // Private members
    property var modelRowState: TableModel.Origin
    property color currentTextColor: normalTextColor

    // Signal for data changed in model.
    signal modelDataChanged
    Component.onCompleted: tableModel.dataChanged.connect(modelDataChanged)
    onModelDataChanged: {
        modelRowState = tableModel.modelRowState(styleData.row);
        switch (modelRowState) {
        case TableModel.Origin:
            itemText.font.strikeout = false
            itemText.font.italic = false
            currentTextColor = normalTextColor
            break
        case TableModel.Modified:
            itemText.font.strikeout = false
            itemText.font.italic = true
            currentTextColor = modifiedTextColor
            break
        case TableModel.Deleted:
            itemText.font.strikeout = true
            itemText.font.italic = false
            currentTextColor = deletedTextColor
            break
        case TableModel.New:
            itemText.font.strikeout = false
            itemText.font.italic = false
            currentTextColor = newItemTextColor
            break
        default:
            break
        }
    }

    Text {
        id: itemText
        text: styleData.value
        color: (modelRowState === TableModel.Origin && styleData.selected) ? highlightTextColor : currentTextColor
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
