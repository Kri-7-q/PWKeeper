import QtQuick 2.0

Rectangle {
    border.color: "lightsteelblue"
    border.width: 1
    gradient: Gradient {
        GradientStop { position: 0; color: "white" }
        GradientStop { position: 1; color: "aliceblue" }
    }
    visible: styleData.selected
}
