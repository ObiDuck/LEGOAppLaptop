import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1

Dialog {
    property int margin: 11
    property int rad: 30
    property int sizeOfItems: 35*2
    height: 200*2
    width: 245*2

    background: MyBackgroundRectangle {}
    header: MyHeaderRectangle {
        width: parent.width
        height: sizeOfItems+4
        MyItemRectangle {
            anchors.left: parent.left
            anchors.leftMargin: (sizeOfItems*7 - sizeOfItems*6 - 2*6)/2
            anchors.top: parent.top
            anchors.topMargin: 2
            id: boxLabelRect
            width: sizeOfItems
            height: sizeOfItems           
            Image {
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "assets/clr_placeBox.png"
            }
        }
        MyItemRectangle {
            id: rowLabelRect
            anchors.left: boxLabelRect.right
            anchors.leftMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 2
            width: sizeOfItems
            height: sizeOfItems
            Image {
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "assets/clr_row.png"
            }
        }
        MyItemRectangle {
            id: columnLabelRect
            anchors.left: rowLabelRect.right
            anchors.leftMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 2
            width: sizeOfItems
            height: sizeOfItems
            Image {
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "assets/clr_column.png"
            }
        }
        MyItemRectangle {
            id: colorLabelRect
            anchors.left: columnLabelRect.right
            anchors.leftMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 2
            width: sizeOfItems
            height: sizeOfItems
            Image {
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "assets/clr_color.png"
            }
        }
        MyItemRectangle {
            id: countLabelRect
            anchors.left: colorLabelRect.right
            anchors.leftMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 2
            width: sizeOfItems
            height: sizeOfItems
            Image {
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "assets/clr_count.png"
            }
        }
        MyItemRectangle {
            id: editLabelRect
            anchors.left: countLabelRect.right
            anchors.leftMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 2
            width: sizeOfItems
            height: sizeOfItems
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "assets/clr_iconEdit.png"
            }
        }
    }
    Text {
        text: "Деталь ещё не расположена"//"Detail not placed yet"
        visible: (lstVw.count == 0)
        anchors.centerIn: parent
    }

    ListView {
        id: lstVw
        width: parent.width
        height: parent.height
        spacing: 3
        clip: true
        model: detailInfoModel
        delegate: Row {
            spacing: 2
            MyItemRectangle {
                width: sizeOfItems
                height: sizeOfItems
                Layout.fillWidth: true
                Text {
                    color: "#00007f"
                    anchors.centerIn: parent
                    text: boxindex
                    fontSizeMode: Text.Fit
                    font.pointSize: 48
                }
            }

            MyItemRectangle {
                width: sizeOfItems
                height: sizeOfItems
                Layout.fillWidth: true
                Text {
                    color: "#00007f"
                    anchors.centerIn: parent
                    text: rowindex
                    fontSizeMode: Text.Fit
                    font.pointSize: 48
                }
            }

            MyItemRectangle {
                width: sizeOfItems
                height: sizeOfItems
                Layout.fillWidth: true
                Text {
                    color: "#00007f"
                    anchors.centerIn: parent
                    text: columnindex
                    font.pointSize: 48
                    fontSizeMode: Text.Fit
                }
            }

            MyItemRectangle {
                width: sizeOfItems
                height: sizeOfItems
                color: colorrole
                Text {
                    text: dbColor.getFamilyColor(colorrole)
                    anchors.centerIn: parent
                    font.pointSize: 48
                    fontSizeMode: Text.Fit
                }

                Layout.fillWidth: true
            }

            MyCountItem {
                width: sizeOfItems
                height: sizeOfItems
                percentCount: count
            }

            MyItemRectangle {
                width: sizeOfItems
                height: sizeOfItems
                Image {
                    anchors.fill: parent
                    source:"assets/clr_edit.png"
                }
                MouseArea {
                    width: parent.width
                    height: parent.height
                    onClicked: {
                        appCore.openPlacementDialog(idindex)
                        close()
                    }
                }
            }
        }
    }
}
