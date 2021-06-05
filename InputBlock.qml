import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
    id: root
    property alias key: key
    property alias value: value
    property alias maxLength: value.maxLength
    key.text: "some"
    RowLayout{
        Text{
            id: key
        }
        FocusScope {
            height: key.height
            width: value.width
            Rectangle {
                anchors.fill: parent
                color: "lightsteelblue"
                border.color: "gray"
            }
            TextEdit {
                property int maxLength: 8
                id: value
                anchors.fill: parent
                focus: true
                onTextChanged: {
                    if (value.length > maxLength){
                        value.text= value.text.substring(0,maxLength);
                        cursorPosition = maxLength
                    }
                }
            }
        }

    }
}
