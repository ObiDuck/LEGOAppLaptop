import QtQuick 2.0
//нет обработки клика
Item {   
    Rectangle {
       id: labelRoot
       border.width: 3
       x: 0
       y: 0
       width: 100
       height: 100
       color: "lightgrey"
       radius: 8

       Rectangle {
            id: rect2
            x: parent.border.width
            y: parent.border.width
            width: 100 - x*2; height: 100 - y*2
            Image {
                fillMode: Image.PreserveAspectCrop
                anchors.centerIn: parent
                source: "assets/001.png"
                width: parent.width
                height: parent.height
            }
            MouseArea {
                id: area
                width: parent.width
                height: parent.height
                hoverEnabled: true
            }
        }
       border.color: area.containsMouse ? "green" : "black"
    }
}
