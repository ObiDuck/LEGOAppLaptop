import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
//мой стилизованный лэйбл
Label {
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    Layout.fillHeight: true
    Layout.fillWidth: true
    background: MyItemRectangle {
          Layout.fillHeight: true
          Layout.fillWidth: true
    }
}
