import QtQuick 2.8
import QtQuick.Controls 2.2
//стилизованный слайдер
Slider {
      id: control
      value: 0.5

      background: Rectangle {
          x: control.leftPadding
          y: control.topPadding + control.availableHeight / 2 - height / 2
          implicitWidth: 200
          implicitHeight: 4
          width: control.availableWidth
          height: 6
          radius: 12
          color: "#ced8ed"
          border.color: "#786ea0"

          Rectangle {
              width: control.visualPosition * parent.width
              height: parent.height
              color: "#00007f"
              radius: 2
          }
      }

      handle: Rectangle {
          x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
          y: control.topPadding + control.availableHeight / 2 - height / 2
          implicitWidth: 26
          implicitHeight: 26
          radius: 13
          color: control.pressed ? "#d4d4ff" : "#ced8ed"
          border.color: "#786ea0"
      }
  }
