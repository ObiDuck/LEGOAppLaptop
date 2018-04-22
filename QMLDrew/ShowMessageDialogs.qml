import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1

Dialog {
    property string newText: ""
    property int iconID: 1
    width: textLabel.width + img.width*1.5
    height: img.height*2
    visible: false
    title: "Системное сообщение"//"System message"
    modal: true
    leftMargin: (parent.width - width)/2
    topMargin: (parent.height - height)/2
    RowLayout {
        Text {
            id: textLabel
            text: newText
        }
        Image {
            id: img
            width: 50
            height: 50
            source: switch (iconID) {
                case 1: "assets/clr_info.png"; break
                case 2: "assets/clr_warning.png"; break
                case 3: "assets/clr_critical.png"; break
                case 4: "assets/clr_question.png"; break
                    }
        }
    }
}

