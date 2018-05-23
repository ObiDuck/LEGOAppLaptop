import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import "./../../../CustomizedItems"
import "./../FilterDialogs"

Dialog {
    property int margin: 11
    property int rad: 30
    property variant myString: []
    property int colorID
    property string quantText: ""
    background: CustomBackgroundRectangle {}

    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight*1.2 + 2 * margin
    modal: true
    leftMargin: (parent.width - width)/2
    topMargin: (parent.height - height)/2
    enter: Transition {
              NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
          }
    onClosed: {
        myString.push(idDetail.text)
        myString.push(colorID)
        myString.push(countSlider.value)
        myString.push(boxTumbler.currentIndex + 1)
        myString.push(rowTumbler.currentIndex + 1)
        myString.push(columnTumbler.currentIndex + 1)
        appExchangePlacementEdit.saveChanges(myString)
        myString.pop()
        myString.pop()
        myString.pop()
        myString.pop()
        myString.pop()
        myString.pop()
    }

    Connections {
        target: appExchangePlacementEdit
        onSendData: {
            idField.text = array[0]
            idDetail.text = array[1]
            colorID = array[2]
            mainColor.color = appExchangePlacementEdit.getColor(array[2])
            countSlider.value = array[3]
            boxTumbler.currentIndex = array[4] - 1
            rowTumbler.currentIndex = array[5] - 1
            columnTumbler.currentIndex = array[6] - 1
        }
        onSendMsg: {
            messageDialog.newText = newText
            messageDialog.open()
        }
    }
    Connections {
        target: appCore
        onShowChooseDetailDialog:
        {
            pickDetailDlg.pressedID = chosenFilter - 1
            pickDetailDlg.open()
        }
    }

    SelectDetailDialog {
        id: pickDetailDlg
        onClosed: idDetail.text = dbDetail4Placement.getID(pickDetailDlg.chosenID)
    }

    ColumnLayout {
        id: mainLayout
        RowLayout {
            ColumnLayout {
                RowLayout {
                    CustomLabel {text: "Цвет"}//"Color:"}
                    Rectangle {
                        id: mainColor
                        width: 100
                        height: 40
                        border.color: "black"                       
                        Layout.fillWidth: true
                        MouseArea {
                            anchors.fill: parent
                            onClicked: showColors.visible = true
                        }
                        Popup {
                            id: showColors
                            y: parent.height
                            width: parent.width
                            height: parent.width
                            visible: false
                            background: CustomBackgroundRectangle {}
                            GridView {
                                width: parent.width
                                height: parent.height
                                clip: true
                                model: colorModel
                                cellHeight: 42
                                cellWidth: 42
                                delegate: Rectangle {
                                    id: labelRoot
                                    width: 40
                                    height: 40
                                    color: "lightgrey"
                                    border.color: area.containsMouse ? "#0000ff" : "#786ea0"
                                    border.width: area.containsMouse ? 3 : 1
                                    radius: area.containsMouse ? 8 : 0
                                    Rectangle {
                                         id: rect2
                                         x: parent.border.width
                                         y: parent.border.width
                                         width: parent.width - x*2; height: parent.height - y*2
                                         Rectangle {
                                             width: parent.width
                                             height: parent.height
                                             color: display
                                             Layout.fillWidth: true
                                         }
                                         MouseArea {
                                             id: area
                                             width: parent.width
                                             height: parent.height
                                             hoverEnabled: true
                                             onClicked: {
                                                 mainColor.color = display
                                                 showColors.visible = false
                                                 colorID = index + 1
                                             }
                                         }
                                     }                                  
                                 }
                            }
                        }
                    }
                }

                GridLayout {
                    columns: 4
                    CustomLabel {text: "Кассетница"}//"Box"}
                    CustomLabel {text: "Ряд"}//"Row"}
                    CustomLabel {id: clmnLabel; text: "Столбец"}//"Clmn"}
                    CustomLabel {text: "Количество деталей"}//"Quantity of details"}
                    Tumbler {
                        id: boxTumbler
                        background: CustomBackgroundRectangle {}
                        Layout.preferredWidth: clmnLabel.width
                        Layout.preferredHeight: clmnLabel.height*6
                        model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"]
                        visibleItemCount: 3
                        /*MouseArea {
                            anchors.fill: parent
                            onWheel: {
                                if (wheel.angleDelta.y < 0)
                                    boxTumbler.currentIndex++
                                else
                                    boxTumbler.currentIndex--
                            }
                        }*/
                    }
                    Tumbler {
                        id: rowTumbler
                        background: CustomBackgroundRectangle {}
                        Layout.preferredWidth: clmnLabel.width
                        Layout.preferredHeight: clmnLabel.height*6
                        model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"]
                        visibleItemCount: 3
                        /*MouseArea {
                            anchors.fill: parent
                            onWheel: {
                                if (wheel.angleDelta.y < 0)
                                    rowTumbler.currentIndex++
                                else
                                    rowTumbler.currentIndex--
                            }
                        }*/
                    }
                    Tumbler {
                        id: columnTumbler
                        background: CustomBackgroundRectangle {}
                        Layout.preferredWidth: clmnLabel.width
                        Layout.preferredHeight: clmnLabel.height*6
                        model: ["1", "2", "3", "4", "5"]
                        visibleItemCount: 3
                        /*MouseArea {
                            anchors.fill: parent
                            onWheel: {
                                if (wheel.angleDelta.y < 0)
                                    columnTumbler.currentIndex++
                                else
                                    columnTumbler.currentIndex--
                            }
                        }*/
                    }
                    CustomSlider {
                        id: countSlider
                        snapMode: Slider.SnapAlways
                        from: 0.0
                        to: 5.0
                        stepSize: 1.0
                    }
                }
                RowLayout {
                    RoundButton {
                        id: deleteButton
                        Image {
                            anchors.fill: parent
                            source: "./../../../assets/clr_delBttn.png"
                        }
                        radius: rad
                        onClicked: appExchangePlacementEdit.deleteItem()
                    }

                    RoundButton {
                        id: firstButton
                        radius: rad
                        Image {
                            anchors.fill: parent
                            source: "./../../../assets/clr_firstBttn.png"
                        }
                        onClicked: {
                            myString.push(idDetail.text)
                            myString.push(colorID)
                            myString.push(countSlider.value)
                            myString.push(boxTumbler.currentIndex + 1)
                            myString.push(rowTumbler.currentIndex + 1)
                            myString.push(columnTumbler.currentIndex + 1)
                            appExchangePlacementEdit.saveChanges(myString)
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            appExchangePlacementEdit.firstItem()
                        }
                    }

                    RoundButton {
                        id: prevButton
                        radius: rad
                        Image {
                            anchors.fill: parent
                            source: "./../../../assets/clr_prevBttn.png"
                        }
                        onClicked: {
                            myString.push(idDetail.text)
                            myString.push(colorID)
                            myString.push(countSlider.value)
                            myString.push(boxTumbler.currentIndex + 1)
                            myString.push(rowTumbler.currentIndex + 1)
                            myString.push(columnTumbler.currentIndex + 1)
                            appExchangePlacementEdit.saveChanges(myString)
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            appExchangePlacementEdit.prevItem()
                        }
                    }

                    TextField {
                        id: idField
                        text: "0"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 50
                        onEditingFinished: {
                            myString.push(idDetail.text)
                            myString.push(colorID)
                            myString.push(countSlider.value)
                            myString.push(boxTumbler.currentIndex + 1)
                            myString.push(rowTumbler.currentIndex + 1)
                            myString.push(columnTumbler.currentIndex + 1)
                            appExchangePlacementEdit.saveChanges(myString)
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            appExchangePlacementEdit.jumpItem(text)
                        }
                    }

                    RoundButton {
                        id: nextButton
                        radius: rad
                        Image {
                            anchors.fill: parent
                            source: "./../../../assets/clr_nextBttn.png"
                        }
                        onClicked: {
                            myString.push(idDetail.text)
                            myString.push(colorID)
                            myString.push(countSlider.value)
                            myString.push(boxTumbler.currentIndex + 1)
                            myString.push(rowTumbler.currentIndex + 1)
                            myString.push(columnTumbler.currentIndex + 1)
                            appExchangePlacementEdit.saveChanges(myString)
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            appExchangePlacementEdit.nextItem()
                        }
                    }

                    RoundButton {
                        id: lastButton
                        radius: rad
                        Image {
                            anchors.fill: parent
                            source: "./../../../assets/clr_lastBttn.png"
                        }
                        onClicked: {
                            myString.push(idDetail.text)
                            myString.push(colorID)
                            myString.push(countSlider.value)
                            myString.push(boxTumbler.currentIndex + 1)
                            myString.push(rowTumbler.currentIndex + 1)
                            myString.push(columnTumbler.currentIndex + 1)
                            appExchangePlacementEdit.saveChanges(myString)
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            appExchangePlacementEdit.lastItem()
                        }
                    }

                    RoundButton {
                        id: addButton
                        radius: rad
                        Image {
                            anchors.fill: parent
                            source: "./../../../assets/clr_addBttn.png"
                        }
                        onClicked: {
                            myString.push(idDetail.text)
                            myString.push(colorID)
                            myString.push(countSlider.value)
                            myString.push(boxTumbler.currentIndex + 1)
                            myString.push(rowTumbler.currentIndex + 1)
                            myString.push(columnTumbler.currentIndex + 1)
                            appExchangePlacementEdit.saveChanges(myString)
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            myString.pop()
                            appExchangePlacementEdit.addItem()
                        }
                    }
                }
            }

            ColumnLayout {
                spacing: 2
                Image {
                    id: imageDetail
                    source: "image://imageProvider/dbDetail/" + idDetail.text
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.preferredWidth: 100
                    Layout.preferredHeight:  100
                    Layout.maximumHeight: 200
                    Layout.maximumWidth: 200
                    MouseArea {
                        anchors.fill: parent
                        onClicked: appCore.openChooseDetailDialog(1)
                    }
                }
                TextField {
                    id: idDetail
                    text: "0"
                    Layout.preferredWidth: 100
                    Layout.maximumWidth: 200
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                }                
            }
        }
    }
}



