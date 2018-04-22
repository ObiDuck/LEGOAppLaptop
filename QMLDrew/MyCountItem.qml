import QtQuick 2.8
//мой элемент для отображения количества деталей
MyItemRectangle {
    property int percentCount: 0
    id: backgroundRect
    width: 50
    height: 50
    MyItemRectangle {
        id: countRect
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.border.width
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.border.width*2
        height: percentCount*(parent.height - parent.border.width*2)/5
        color: switch (percentCount)
               {
               case 0: "lightgrey"; break
               case 1: "red"; break
               case 2: "orange"; break
               case 3: "yellow"; break
               case 4: "lightgreen"; break
               case 5: "green"; break
               }
    }
}




