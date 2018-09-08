import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import "./../../CustomizedItems"

Dialog {
    property int margin: 11
    property int rad: 30
    property int sizeOfItems: 70
    property int detail
    height: 400
    width: 490

    background: CustomBackgroundRectangle {}
    header: CustomHeaderRectangle {
        width: parent.width
        height: sizeOfItems+4
        CustomItemRectangle {
            anchors.left: parent.left
            anchors.leftMargin: (sizeOfItems - 12)/2
            anchors.top: parent.top
            anchors.topMargin: 2
            id: boxLabelRect
            width: sizeOfItems
            height: sizeOfItems           
            Image {
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "./../../assets/clr_placeBox.png"
            }
        }
        CustomItemRectangle {
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
                source: "./../../assets/clr_row.png"
            }
        }
        CustomItemRectangle {
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
                source: "./../../assets/clr_column.png"
            }
        }
        CustomItemRectangle {
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
                source: "./../../assets/clr_color.png"
            }
        }
        CustomItemRectangle {
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
                source: "./../../assets/clr_count.png"
            }
        }
        CustomItemRectangle {
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
                source: "./../../assets/clr_iconEdit.png"
            }
        }
    }//header>
    //TODO: add image
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
            CustomItemRectangle {
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

            CustomItemRectangle {
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

            CustomItemRectangle {
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

            CustomItemRectangle {
                width: sizeOfItems
                height: sizeOfItems
                color: colorrole
                Text {
                    text: appExchangeColourEdit.getFamilyColor(colorrole)
                    anchors.centerIn: parent
                    font.pointSize: 48
                    fontSizeMode: Text.Fit
                }

                Layout.fillWidth: true
            }

            CustomCountItem {
                width: sizeOfItems
                height: sizeOfItems
                percentCount: count
            }

            CustomItemRectangle {
                width: sizeOfItems
                height: sizeOfItems
                Image {
                    anchors.fill: parent
                    source:"./../../assets/clr_edit.png"
                }
                MouseArea {
                    width: parent.width
                    height: parent.height
                    onClicked: {
                        appCore.openPlacementDialog(idindex, true)
                        close()
                    }
                }
            }
        }
    }//mainList>

    footer: CustomHeaderRectangle {
        width: parent.width
        height: sizeOfItems+4
        CustomItemRectangle {
            anchors.left: parent.left
            anchors.leftMargin: (sizeOfItems - 12)/2
            anchors.top: parent.top
            anchors.topMargin: 2
            id: addNewDetail
            width: sizeOfItems
            height: sizeOfItems
            Image {
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "./../../assets/clr_edit.png"
            }
            MouseArea {
                width: parent.width
                height: parent.height
                onClicked: {
                    appCore.openDetailDialog(detail)
                    close()
                }
            }
        }
    }

}
