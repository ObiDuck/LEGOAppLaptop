import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1

Dialog {
    //диалог выбора фильтра цвета
    property int margin: 11
    property int rad: 30
    property int chosenFilter: 0

    background: MyBackgroundRectangle {}
    width: 940
    height: 550
    modal: true
    leftMargin: (parent.width - width)/2
    topMargin: (parent.height - height)/2
    enter: Transition {
              NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
          }

    GridView {
        width: parent.width
        height: parent.height
        clip: true
        model: colorFilterModel
        cellHeight: 100
        cellWidth: 100
        delegate: Rectangle {
            id: labelRoot
            width: 100
            height: 100
            color: "lightgrey"
            border.color: area.containsMouse ? "#0000ff" : "#786ea0"
            border.width: area.containsMouse ? 3 : 1
            radius: area.containsMouse ? 8 : 0
            Rectangle {
                 id: rect2
                 x: parent.border.width
                 y: parent.border.width
                 width: parent.width - x*2; height: parent.height - y*2
                 Rectangle {
                     width: parent.width
                     height: parent.height
                     color: display
                     Layout.fillWidth: true
                     Text {
                         text: dbColor.getFamilyColor(index)
                         anchors.centerIn: parent
                         font.pointSize: 48
                         fontSizeMode: Text.Fit
                     }
                 }
                 MouseArea {
                     id: area
                     width: parent.width
                     height: parent.height
                     hoverEnabled: true
                     onClicked: {
                         chosenFilter = index + 1
                         close()
                     }
                 }
             }
         }
    }
}

