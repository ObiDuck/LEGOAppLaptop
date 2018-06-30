import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import "./../../../CustomizedItems"

Dialog {
    //диалог редактирования таблицы подкатегории
    property int margin: 11
    property int rad: 30
    property string newImage: ""
    property variant myString: []

    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight*1.2
    //размещение окна по центру экрана
    leftMargin: (parent.width - width)/2
    topMargin: (parent.height - height)/2
    background: CustomBackgroundRectangle {}
    modal: true
    enter: Transition {
              NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
          }
    //сохранение изменений при закрытии окна
    onClosed: {
        //имя подкатегории
        myString.push(nameField.text)
        //индекс категории
        myString.push(categoryBox.currentIndex+1)
        appExchangeSubCategoryEdit.saveChanges(myString)
        myString.pop()
        myString.pop()
        newImage = ""
    }
    //обработка сигналов
    Connections {
        target: appExchangeSubCategoryEdit
        onSendData: {
            //порядковый номер подкатегории
            idField.text = array[0]
            //имя подкатегории
            nameField.text = array[2]
            //невидимое поле с путем к картинке
            pathField.text = array[4]
            //индекс текущей категории
            categoryBox.currentIndex = array[3] - 1
            imageSubCategory.source = "image://imageProvider/dbSubCategory/" + array[1]
        }
        //окно сообщения
        onSendMsg: {
            messageDialog.newText = qsTr(newText)
            messageDialog.open()
        }
    }

    //файл-диалог для загрузки иконки подкатегории
    CustomFileDialog {
        id: iconDlg
        nameFilters: ["PNG images (*.png)"]
        onAccepted:
        {
            newImage = "temp" + iconDlg.file
            pathField.text = "loaded"
            appExchangeSubCategoryEdit.setIcon(iconDlg.file)//передаем полученный адрес базы
        }
    }

    RowLayout {
        id: mainLayout
        ColumnLayout {//столбец со всеми элементами помимо картинки
            GridLayout {//сетка с управляющими элементами
                columns: 2
                CustomLabel {text: "Имя"}//"Name"}
                TextField {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: nameField
                    placeholderText: "Имя подкатегории"//"name of subcategory"
                }
                CustomLabel {text: "Категория"}//"Category"}
                ComboBox {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: categoryBox
                    model: categoryModel
                    textRole: "display"
                    background: Rectangle {
                              implicitWidth: 120
                              implicitHeight: 40
                              border.color: "#786ea0"
                              border.width: 1
                              radius: 2
                          }
                }
                TextField {
                    visible: false
                    Layout.fillHeight: true; Layout.fillWidth: true
                    id: pathField
                    placeholderText: "Путь к иконке"//"path to icon"
                    enabled: false
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
                    onClicked: appExchangeSubCategoryEdit.deleteItem()
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
                        myString.push(categoryBox.currentIndex+1)
                        appExchangeSubCategoryEdit.saveChanges(myString)
                        myString.pop()
                        myString.pop()
                        newImage = ""
                        appExchangeSubCategoryEdit.firstItem()
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
                        myString.push(categoryBox.currentIndex+1)
                        appExchangeSubCategoryEdit.saveChanges(myString)
                        myString.pop()
                        myString.pop()
                        newImage = ""
                        appExchangeSubCategoryEdit.prevItem()
                    }
                }

                TextField {
                    id: idField
                    text: "0"
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 50
                    onEditingFinished: {
                        myString.push(nameField.text)
                        myString.push(categoryBox.currentIndex+1)
                        appExchangeSubCategoryEdit.saveChanges(myString)
                        myString.pop()
                        myString.pop()
                        newImage = ""
                        appExchangeSubCategoryEdit.jumpItem(idField.text)
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
                        myString.push(categoryBox.currentIndex+1)
                        appExchangeSubCategoryEdit.saveChanges(myString)
                        myString.pop()
                        myString.pop()
                        newImage = ""
                        appExchangeSubCategoryEdit.nextItem()
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
                        myString.push(categoryBox.currentIndex+1)
                        appExchangeSubCategoryEdit.saveChanges(myString)
                        myString.pop()
                        myString.pop()
                        newImage = ""
                        appExchangeSubCategoryEdit.lastItem()
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
                        myString.push(categoryBox.currentIndex+1)
                        appExchangeSubCategoryEdit.saveChanges(myString)
                        myString.pop()
                        myString.pop()
                        newImage = ""
                        appExchangeSubCategoryEdit.addItem()
                    }
                }
            }
        }

        Image {
            id: imageSubCategory
            source: "./../../../assets/clr_iconPic.png" //: "image://imageProvider/dbSubCategory/" + array[0] + newImage
            cache: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.preferredWidth: 100
            Layout.preferredHeight:  100
            MouseArea {
                anchors.fill: parent
                onClicked: iconDlg.open()
            }
        }
    }
}
