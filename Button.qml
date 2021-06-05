import QtQuick 2.0
Item{
    id: root
    property alias color: rectButton.color
    property alias text: label.text
    signal clicked
    Rectangle {
        id: rectButton;
        anchors.fill: root
        color: "lightsteelblue"; border.color: "slategrey"
        Text {id: label;anchors.centerIn: parent;text: "Start"}
        MouseArea {anchors.fill: parent;onClicked: {root.clicked()}
        }
    }
}
