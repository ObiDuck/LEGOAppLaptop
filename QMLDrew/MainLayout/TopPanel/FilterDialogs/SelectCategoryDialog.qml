import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import "./../../../CustomizedItems"

Dialog {
    //диалог выбора фильтра подкатегории
    property int margin: 11
    property int rad: 30
    property int chosenFilter: 0 //переменная хранения выбранного фильтра

    background: CustomBackgroundRectangle {}
    width: 740
    height: 430
    modal: true
    leftMargin: (parent.width - width)/2
    topMargin: (parent.height - height)/2

    enter: Transition {
              NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
          }

    GridView {//сетка выводимых иконок фильтров
        width: parent.width
        height: parent.height
        model: catModel
        clip: true
        cellHeight: 100
        cellWidth: 100
        delegate: Rectangle {//фоновый прямоугольник
            width: 100
            height: 100
            color: "lightgrey"
            border.color: area.containsMouse ? "#0000ff" : "#786ea0" //реагирование цветов на наведение мыши
            border.width: area.containsMouse ? 3 : 1
            radius: area.containsMouse ? 8 : 0
            Rectangle {//прямоугольник с изображением
                 id: rect2
                 x: parent.border.width
                 y: parent.border.width
                 width: parent.width - x*2; height: parent.height - y*2
                 Image {
                     fillMode: Image.PreserveAspectCrop
                     anchors.centerIn: parent
                     source: "image://imageProvider/dbSubCategory/" + display
                     width: parent.width
                     height: parent.height
                 }
                 MouseArea {
                     id: area
                     width: parent.width
                     height: parent.height
                     hoverEnabled: true
                     onClicked: {
                         //сохраняем индекс выбранного итема и закрываем окно
                         chosenFilter = display
                         close()
                     }
                 }
             }
         }
    }
}
