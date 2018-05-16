import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import "./../../../CustomizedItems"

Dialog {
    property int margin: 11
    property int rad: 30
    property string newImage: ""
    property variant myString: []
    property int idCategory

    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight*1.2 + 2 * margin
    modal: true
    leftMargin: (parent.width - width)/2
    topMargin: (parent.height - height)/2
    enter: Transition {
              NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
          }
    background: CustomBackgroundRectangle {}

    onClosed: {
        myString.push(nameField.text)
        myString.push(idCategory)
        dbDetail.saveChanges(myString)
        myString.pop()
        myString.pop()
        myString.pop()
    }

   Connections {
        target: dbDetail
        onSendData: {
            idField.text = array[0]
            nameField.text = array[1]
            idCategory = array[2]
            pathField.text = array[3]
        }
        onSendMsg: {
            messageDialog.newText = newText
            messageDialog.open()
        }
    }

    CustomFileDialog {id: iconDlg
        nameFilters: ["PNG images (*.png)"]
        onAccepted:
        {
            newImage = "temp" + iconDlg.file
            pathField.text = "loaded"
            dbDetail.setIcon(iconDlg.file)
        }
    }
    ColumnLayout {
        id: mainLayout
        RowLayout {
            GridLayout {
                columns: 2
                CustomLabel {text: "Имя:"}//"Name:"}
                TextField {
                    id: nameField
                    Layout.alignment: Qt.AlignRight
                    Layout.fillWidth: true
                    Layout.preferredWidth: 50
                    placeholderText: "Detail name"
                }
                CustomLabel {text: "Подкатегория:"}//"SubCategory:"}
                Image {
                    id: mainImgSubCat
                    source: "image://imageProvider/dbSubCategory/" + idCategory
                    cache: false
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.preferredWidth: 50
                    Layout.preferredHeight:  50
                    MouseArea {
                        id: subCatArea
                        anchors.fill: parent
                        onClicked: showCats.visible = true
                    }
                }
                Popup {
                    id: showCats
                    x: mainImgSubCat.x
                    y: parent.height
                    width: parent.width*2
                    height: parent.width
                    visible: false
                    background: CustomBackgroundRectangle {}
                    GridView {
                        width: parent.width
                        height: parent.height
                        clip: true
                        model: subCatModel
                        cellHeight: 60
                        cellWidth: 60
                        delegate: Rectangle {
                            id: labelRoot
                            width: 60
                            height: 60
                            color: "lightgrey"
                            border.color: popupSubCatImgArea.containsMouse ? "#0000ff" : "#786ea0"
                            border.width: popupSubCatImgArea.containsMouse ? 3 : 1
                            radius: popupSubCatImgArea.containsMouse ? 8 : 0
                            Rectangle {
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
                                     id: popupSubCatImgArea
                                     width: parent.width
                                     height: parent.height
                                     hoverEnabled: true
                                     onClicked: {
                                        idCategory = display
                                        showCats.visible = false
                                     }
                                 }
                             }
                         }
                    }
                }
            }
            ColumnLayout {
                Image {
                    id: imageDetail
                    source:  (pathField.text === "") ? "./../../../assets/clr_iconPic.png" : "image://imageProvider/dbDetail/" + idField.text + newImage
                    cache: false
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.preferredWidth: 100
                    Layout.preferredHeight:  100
                    MouseArea {
                        anchors.fill: parent
                        onClicked: iconDlg.open()
                    }
                }

                TextField {
                    id: pathField
                    placeholderText: "Путь к иконке"//"path to icon"
                    enabled: false
                    visible: false
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                }
            }
        }
        RowLayout {
            //ряд с кнопками переключения
            RoundButton {
                id: deleteButton
                Image {
                    anchors.fill: parent
                    source: "./../../../assets/clr_delBttn.png"
                }
                radius: rad
                onClicked: dbDetail.deleteItem()
            }

            RoundButton {
                id: firstButton
                radius: rad
                Image {
                    anchors.fill: parent
                    source: "./../../../assets/clr_firstBttn.png"
                }
                onClicked: {
                    myString.push(nameField.text)
                    myString.push(idCategory)
                    dbDetail.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    newImage = ""
                    dbDetail.firstItem()
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
                    myString.push(nameField.text)
                    myString.push(idCategory)
                    dbDetail.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    newImage = ""
                    dbDetail.prevItem()
                }
            }

            TextField {
                id: idField
                text: "0"
                Layout.preferredHeight: 30
                Layout.preferredWidth: 50
                onEditingFinished: {
                    myString.push(nameField.text)
                    myString.push(idCategory)
                    dbDetail.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    newImage = ""
                    dbDetail.jumpItem(text)
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
                    myString.push(nameField.text)
                    myString.push(idCategory)
                    dbDetail.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    newImage = ""
                    dbDetail.nextItem()
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
                    myString.push(nameField.text)
                    myString.push(idCategory)
                    dbDetail.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    newImage = ""
                    dbDetail.lastItem()
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
                    myString.push(nameField.text)
                    myString.push(idCategory)
                    dbDetail.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    newImage = ""
                    dbDetail.addItem()
                }
            }
        }
    }
}
