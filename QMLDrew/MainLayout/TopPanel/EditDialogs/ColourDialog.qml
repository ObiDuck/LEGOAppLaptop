import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import "./../../../CustomizedItems"

Dialog {
    //диалог редактирования таблицы цвета в БД
    property int margin: 11
    property int rad: 30
    property variant myString: []

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
        myString.push(familyBox.currentIndex+1)
        myString.push(colorDlg.color)
        appExchangeColourEdit.saveChanges(myString)
        myString.pop()
        myString.pop()
        myString.pop()
    }

    Connections {
        target: appExchangeColourEdit
        onSendData: {
            idField.text = array[0]
            nameField.text = array[2]
            familyBox.currentIndex = array[3] - 1
            colorDlg.color = array[4]
        }
        onSendMsg: {
            messageDialog.newText = newText
            messageDialog.open()
        }
    }
    //диалог для выбора цвета
    CustomColorDialog {id: colorDlg}

    ColumnLayout{
        id: mainLayout
        GridLayout {  //сетка с данными для таблицы
            columns: 2
            CustomLabel {text: "Имя"}//"Name"}
            TextField {
                id: nameField
                placeholderText: "Имя цвета"//"color name"
                Layout.fillWidth: true
            }
            CustomLabel {text: "Семейство цвета"}//"Color Family"}
            ComboBox {
                id: familyBox
                model: colorFamilyModel
                Layout.fillWidth: true
                textRole: "display"
                background: Rectangle {
                          implicitWidth: 120
                          implicitHeight: 40
                          border.color: "#786ea0"
                          border.width: 1
                          radius: 2
                      }
            }
            CustomLabel {text: "Цвет"}//"Color"}
            Rectangle {
                width: 100
                height: 40
                border.color: "black"
                color: colorDlg.color
                Layout.fillWidth: true
                MouseArea {
                    anchors.fill: parent
                    onClicked: colorDlg.open()
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
                onClicked: appExchangeColourEdit.deleteItem()
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
                    myString.push(familyBox.currentIndex+1)
                    myString.push(colorDlg.color)
                    appExchangeColourEdit.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    appExchangeColourEdit.firstItem()
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
                    myString.push(familyBox.currentIndex+1)
                    myString.push(colorDlg.color)
                    appExchangeColourEdit.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    appExchangeColourEdit.prevItem()
                }
            }

            TextField {
                id: idField
                text: "0"
                Layout.preferredHeight: 30
                Layout.preferredWidth: 50
                onEditingFinished: {
                    myString.push(nameField.text)
                    myString.push(familyBox.currentIndex+1)
                    myString.push(colorDlg.color)
                    appExchangeColourEdit.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    appExchangeColourEdit.jumpItem(idField.text)
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
                    myString.push(familyBox.currentIndex+1)
                    myString.push(colorDlg.color)
                    appExchangeColourEdit.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    appExchangeColourEdit.nextItem()
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
                    myString.push(familyBox.currentIndex+1)
                    myString.push(colorDlg.color)
                    appExchangeColourEdit.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    appExchangeColourEdit.lastItem()
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
                    myString.push(familyBox.currentIndex+1)
                    myString.push(colorDlg.color)
                    appExchangeColourEdit.saveChanges(myString)
                    myString.pop()
                    myString.pop()
                    myString.pop()
                    appExchangeColourEdit.addItem()
                }
            }
        }
    }
}
