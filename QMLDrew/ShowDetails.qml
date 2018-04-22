import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1

//ScrollView {


GridView {

property int chosenID
property int coorX: 0
property int coorY: 0
property int iconWidth: 100
property int iconHeight: 100
property bool flag: false
property real iconSize
property point pnt
property string name: "0"
property int pressedID: -1

width: parent.width
height: parent.height

id: viewGR
model: showDetailsModel
cellHeight: 100 + 100*iconSize
cellWidth: 100 + 100*iconSize
snapMode: GridView.SnapToRow
delegate: Rectangle {
    id: labelRoot
    width: 100 + 100*iconSize
    height: 100 + 100*iconSize
    color: "lightgrey"
    border.color: (pressedID == index) ? "#00007f" : area.containsMouse ? "#0000ff" : "#bfe1e6"
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
             source: (flag) ? "image://imageProvider/" + name + "dbGetDetail/" + display : "assets/clr_iconPic.png"
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
                 pnt = viewGR.mapFromItem(parent,x,y)
                 if ((pnt.y + 200*2) < viewGR.height)
                     coorY = pnt.y - 3
                 else
                     coorY = pnt.y - 3 - 100*2
                 if ((pnt.x + labelRoot.width - x + 35*7*2) < viewGR.width)
                     coorX = pnt.x + labelRoot.width - x - 3
                 else
                     coorX = pnt.x - 35*7*2 - 3
                 pressedID = index
                 appCore.openDetailInfo(chosenID)
             }          
         }

     }
 }
}
