import QtQuick 2.8
import QtQuick.Controls 2.2
//стилизованный свитч
Switch {
      id: control

      indicator: Rectangle {
          implicitWidth: 48
          implicitHeight: 13
          x: control.leftPadding
          y: parent.height / 2
          radius: 13
          color: control.checked ? "#00007f" : "#ced8ed"
          border.color: control.checked ? "#786ea0" : "#786ea0"

          Rectangle {
              x: control.checked ? parent.width - width : 0
              y: -parent.height/2
              width: 26
              height: 26
              radius: 13
              color: control.down ? "#d4d4ff" : "#ced8ed"
              border.color: "#786ea0"
          }
      }
}
