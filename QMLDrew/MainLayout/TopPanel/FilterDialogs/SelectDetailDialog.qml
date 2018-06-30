import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import "./../../../CustomizedItems"

Dialog {
    id: dlg
    //диалог с деталями
    property int margin: 11
    property int rad: 30
    property int chosenID
    property string name: "0"
    property int pressedID: -1

    Connections {
        target: appCore
        onRefreshDetails4Placement: {
            name = newText
            //pressedID = chosenFilter - 1
        }
    }

    width: 1050
    height: 640
    x: (parent.parent.width - width) / 2
    y: (parent.parent.height - height) / 2
    modal: true
    enter: Transition {
              NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
          }
    background: CustomBackgroundRectangle {}
   /// RowLayout {
        GridView {//сетка с иконками деталей
            x: 0
            y: 0
            width: 520
            height: 640
            clip: true
            model: detailModel
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
                     Image {
                         fillMode: Image.PreserveAspectCrop
                         anchors.centerIn: parent
                         //source: "image://imageProvider/dbGetDetail/" + display
                         source: "image://imageProvider/" + name + "dbFiltCat/" + display
                         width: parent.width
                         height: parent.height
                     }
                     MouseArea {
                         id: area
                         width: parent.width
                         height: parent.height
                         hoverEnabled: true
                         onClicked: {
                             chosenID = display
                             dlg.close()
                         }
                     }
                 }
             }
        }

        GridView {//сетка с иконками деталей
            width: 520
            height: 640
            x: 530
            y: 0
            clip: true
            model: addCatFilterModel
            cellHeight: 100
            cellWidth: 100
            delegate: Rectangle {
                width: 100
                height: 100
                color: "lightgrey"
                //border.color: catArea.containsMouse ? "#0000ff" : "#786ea0"
                border.color: (pressedID == index) ? "#ff007f" : catArea.containsMouse ? "#0000ff" : "#bfe1e6"
                border.width: catArea.containsMouse ? 3 : 1
                radius: catArea.containsMouse ? 8 : 0

                Rectangle {
                     x: parent.border.width
                     y: parent.border.width
                     width: parent.width - x*2; height: parent.height - y*2
                     Image {
                         fillMode: Image.PreserveAspectCrop
                         anchors.centerIn: parent
                         //source: "image://imageProvider/dbGetDetail/" + display
                         source: "image://imageProvider/dbSubCategory/" + display
                         width: parent.width
                         height: parent.height
                     }
                     MouseArea {
                         id: catArea
                         width: parent.width
                         height: parent.height
                         hoverEnabled: true
                         onClicked: {
                             appCore.openChooseDetailDialog(display)
                             dlg.close()
                         }
                     }
                 }
             }
        }

}
