import QtQuick 2.8
//мой элемент для отображения количества деталей
CustomItemRectangle {
    property int percentCount: 0
    id: backgroundRect
    width: 50
    height: 50
    CustomItemRectangle {
        id: redRect
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height/5
        color: (percentCount > 0) ? "red" : "lightgrey"
    }
    CustomItemRectangle {
        id: orangeRect
        anchors.bottom: redRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height/5
        color: (percentCount > 1) ? "orange" : "lightgrey"
    }
    CustomItemRectangle {
        id: yellowRect
        anchors.bottom: orangeRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height/5
        color: (percentCount > 2) ? "yellow" : "lightgrey"
    }
    CustomItemRectangle {
        id: lightGreenRect
        anchors.bottom: yellowRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height/5
        color: (percentCount > 3) ? "lightgreen" : "lightgrey"
    }
    CustomItemRectangle {
        id: greenRect
        anchors.bottom: lightGreenRect.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height/5
        color: (percentCount > 4) ? "green" : "lightgrey"
    }
}
