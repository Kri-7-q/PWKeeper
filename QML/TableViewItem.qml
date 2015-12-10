/**
  * TableView item delegate
  * --------------------------
  * Draws a TableView cell. It shows the text of a cell with different colors.
  * It has colors for a selected Item, deleted Item, modified Item and a new Item.
  * In addition to the colors it shows deleted text strikeout and modified text
  * is shown in italic.
  * To update the color and font if catches the models 'dataChanged()' signal.
  * A Item is connected to the dataStyelChanged() signal if it is selected.
  * These connection will be released when the selection has changed.
  * The dataStyleChanged() signal is emitted by the model when the state of a
  * item has changed (deleted, modified, new).
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

    Text {
        id: itemText
        text: styleData.value
        color: tableViewItemPrivate.textColor()
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


    // Private member
    QtObject {
        id: tableViewItemPrivate
        property int modelRowState: TableModel.Origin
        property bool selected: styleData.selected

        /* Signal for data changed in model.
      TableModel sends a signal when the state of a model row
      changes. State of a model row is 'Origin'. This state
      changes when a Account object was modified or deleted.
      When a row in TableView is selected the signal is
      connected to function 'updateStyle()'. This function
      changes font style and font color of an item.
    */
        onSelectedChanged: {
            if (selected === true) {
                tableViewModel.dataStyleChanged.connect(updateStyle)
            } else {
                tableViewModel.dataStyleChanged.disconnect(updateStyle)
            }
        }

        function updateStyle() {
            modelRowState = listViewController.modelRowState(styleData.row)
            switch (modelRowState) {
            case TableModel.Origin:
                itemText.font.strikeout = false
                itemText.font.italic = false
                break
            case TableModel.Modified:
                itemText.font.strikeout = false
                itemText.font.italic = true
                break
            case TableModel.Deleted:
                itemText.font.strikeout = true
                itemText.font.italic = false
                break
            case TableModel.New:
                itemText.font.strikeout = false
                itemText.font.italic = false
                break
            default:
                break
            }
        }

        // Function to determine current text color.
        function textColor() {
            var color
            switch (modelRowState) {
            case TableModel.Origin:
                if (styleData.selected) {
                    color = highlightTextColor
                } else {
                    color = normalTextColor
                }
                break
            case TableModel.New:
                color = newItemTextColor
                break
            case TableModel.Modified:
                color = modifiedTextColor
                break
            case TableModel.Deleted:
                color = deletedTextColor
                break
            default:
                color = normalTextColor
                break
            }

            return color
        }
    } // END - QtObject (Private)
}
