import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

ButtonStyle {
    id: buttonStyleObject

    property color color: "steelblue"
    property color highlightColor: "lightblue"
    property int borderWidth: 1
    property color borderColor: "black"
    property int borderFocusWidth: 3
    property string fontFamily: "Arial"
    property int fontSize: 14
    property int fontWeight: Font.Normal
    property color textColor: "black"
    property color focusColor: "darkgray"

    background: buttonVisibleRect
    label: buttonLabel

    // -------------------------------------------
    // Bachground
    Component {
        id: buttonVisibleRect
        Rectangle {
            color: control.hovered ? buttonStyleObject.highlightColor : buttonStyleObject.color
            border.color: control.activeFocus ? buttonStyleObject.focusColor : buttonStyleObject.borderColor
            border.width: control.activeFocus ? buttonStyleObject.borderFocusWidth : buttonStyleObject.borderWidth
        }
    }

    // --------------------------------------------
    // Label
    Component {
        id: buttonLabel
        Text {
            text: control.text
            color: buttonStyleObject.textColor
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font {
                family: buttonStyleObject.fontFamily
                pixelSize: buttonStyleObject.fontSize
                weight: buttonStyleObject.fontWeight
            }
        }
    }
}
