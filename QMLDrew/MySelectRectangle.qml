import QtQuick 2.0

Rectangle {
    property int outerWidthVal: 60
    property int outerHeightVal: 60
    color: "lightgrey"
    radius: 8
    width: outerWidthVal
    height: outerHeightVal
       border {
           width: 3;
           color:  my_area.containsMouse ? "#0000ff" : "#ced8ed";
       }
       Rectangle {
            x: parent.border.width
            y: parent.border.width
            width: parent.width - x*2; height: parent.height - y*2
            MouseArea {
                id: my_area
                width: parent.width
                height: parent.height
                hoverEnabled: true
            }
       }
}
