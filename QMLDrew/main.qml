import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1

ApplicationWindow {
    property bool db_connected: false
    property int xCoor: 0
    property int yCoor: 0
    property int iconWidth: 50
    property string place: ""
    property string template: "0"

    id: rootWindow
    visible: true
    width: 1368
    height: 768
    title: qsTr("LEGO Приложение")//("LEGO Storage App")
    background: MyBackgroundRectangle {}

    //обработка сигналов от приложения
    Connections {
        target: appCore
        //текст статус бара
        onChangeStatusBar: statusLabel.text = newText
        //вывод сообщений
        onShowMessage: {
            messageDialog.newText = qsTr(newText)
            messageDialog.iconID = typeOfMsg
            messageDialog.open()
        }
        //открытие диалога редактирования таблицы подкатегорий
        onShowSubCategoryDialog: {
            categoryDialog.open()
        }

        //открытие диалога редактирования таблицы цвета
        onShowColorDialog: {
            colorDialog.open()
        }
        //открытие диалога редактирования таблицы деталей
        onShowDetailDialog: {
            detailDialog.open()
        }
        //открытие диалога редактирования таблицы расположения
        onShowPlacementDialog: {
            placementDialog.open()
        }
        //вызов получения списка деталей для отображения на главном окне
        onShowDetails: {
            db_connected = true
            mainDetails.flag = true
        }
        //открытие диалога фильтра подкатегорий
        onShowChooseCatFilterDialog: {
            chooseCatFilterDialog.open()
        }
        //открытие диалога фильтра цвета
        onShowChooseColorFilterDialog: {
            colorFilterDlg.open()
        }
        onSetName:
        {
            //workaround для постоянного обновления изображения при изменении содержания, но не индекса
            mainDetails.name = newText            
        }
        onShowDetailInfo:
        {
            //вычисление положения диалога с информацией о детали
            detailInfoDlg.y = mainDetails.coorY
            detailInfoDlg.x = mainDetails.coorX
            //открытие диалога
            detailInfoDlg.open()
        }
    }

    //верхнее меню
    header:  ToolBar {
        background: MyHeaderRectangle {}
        id: filtersToolBar
        height: 75
        Row {
            spacing: 2
            //Кнопка меню с открытием/закрытием соединения к БД
            ToolButton {
                Image {
                    anchors.fill: parent
                    source:"assets/clr_openDB.png"
                }
                width: 75
                height: 75
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Соединение с Базой Данных:")//("Connection to DB")
                onClicked: connectionMenu.open()
            }
            //Кнопка меню редактирования таблиц
            ToolButton {
                Image {
                    anchors.fill: parent
                    source:"assets/clr_settings.png"
                }
                width: 75
                height: 75
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Редактировать таблицу в БД")//("Edit Table in DB")
                onClicked: {
                    editMenu.leftMargin = width + spacing
                    editMenu.open()
                }
            }

            ToolSeparator {}
            //Кнопка фильтров категории
            ToolButton {
                width: 75
                height: 75
                id: categoriesToolButton
                Image {
                    anchors.fill: parent
                    source:"assets/clr_catFilter.png"
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Фильтр по категориям")//("Categories Filter")
                onClicked: {
                    appCore.openChooseCatFilterDialog()
                }
            }
            //Свитч для быстрого выключения фильтра
            MySwitch {                
                id: catFilterSwitch
                onCheckedChanged: {
                    if (checked == true) {
                        catFilterSwitch.checkable = true
                    }
                    else {
                        catFilterSwitch.checkable = false
                        chooseCatFilterDialog.chosenFilter = 0
                        appCore.getDetails(chooseCatFilterDialog.chosenFilter,colorFilterDlg.chosenFilter, "0/0/0")
                    }
                }
                width: 75
                height: 75
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Фильтр Вкл/Выкл")//("Filter On/Off")
            }

            ToolSeparator { }
            //Кнопка фильтров цвета
            ToolButton {
                width: 75
                height: 75
                id: colorsToolButton
                Image {
                    anchors.fill: parent
                    source: "assets/clr_colorFilter.png"
                }
                onClicked:  appCore.openChooseColorFilterDialog()
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Фильтр по цвету")//("Colors Filter")
            }
            //Свитч для быстрого выключения фильтра
            MySwitch {
                width: 75
                height: 75
                id: colFilterSwitch
                onCheckedChanged: {
                    if (checked == true) {
                        colFilterSwitch.checkable = true
                    }
                    else {
                        colFilterSwitch.checkable = false
                        colorFilterDlg.chosenFilter = 0
                        appCore.getDetails(chooseCatFilterDialog.chosenFilter, colorFilterDlg.chosenFilter, "0/0/0")
                    }
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Фильтр Вкл/Выкл")//("Filter On/Off")
            }

            ToolSeparator { }

            ToolButton {
                width: 75
                height: 75
                //enabled: false
                Image {
                    anchors.fill: parent
                    source: "assets/clr_box.png"
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Фильтр по расположению")//("Placement Filter")
            }
            //Тамблеры для кассетницы, её ряда и столбца
            Tumbler {
                width: 75
                height: 75
                id: boxTumbler
               // width: colorsToolButton.width
               // height: colorsToolButton.height
                model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"]
                visibleItemCount: 1
                onMovingChanged: if (placeFilterSwitch.checked == true)
                                 {
                                     place = (boxTumbler.currentIndex + 1) + "/" + (rowTumbler.currentIndex + 1) + "/" + (columnTumbler.currentIndex + 1)
                                     appCore.getDetails(0,0, place)
                                 }
               /* MouseArea {
                    anchors.fill: parent
                    onWheel: {
                        if (wheel.angleDelta.y < 0)
                            boxTumbler.currentIndex++
                        else
                            boxTumbler.currentIndex--
                        if (placeFilterSwitch.checked == true)
                        {
                            place = (boxTumbler.currentIndex + 1) + "/" + (rowTumbler.currentIndex + 1) + "/" + (columnTumbler.currentIndex + 1)
                            appCore.getDetails(0,0, place)
                        }
                    }
                }*/
               /* delegate: Text {
                    text: modelData
                    opacity: 0.4 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6
                }*/
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Номер кассетницы")//("Box Number")
            }
            Tumbler {
                width: 75
                height: 75
                id: rowTumbler
                //width: colorsToolButton.width
               // height: colorsToolButton.height
                model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"]
                visibleItemCount: 1
                MouseArea {
                    anchors.fill: parent
                    onWheel: {
                        if (wheel.angleDelta.y < 0)
                            rowTumbler.currentIndex++
                        else
                            rowTumbler.currentIndex--
                        if (placeFilterSwitch.checked == true)
                        {
                            place = (boxTumbler.currentIndex + 1) + "/" + (rowTumbler.currentIndex + 1) + "/" + (columnTumbler.currentIndex + 1)
                            appCore.getDetails(0,0, place)
                        }
                    }
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Номер ряда")//("Row Number")
            }
            Tumbler {
                width: 75
                height: 75
                id: columnTumbler
             //   width: colorsToolButton.width
             //   height: colorsToolButton.height
                model: ["1", "2", "3", "4", "5"]
                visibleItemCount: 1
                MouseArea {
                    anchors.fill: parent
                    onWheel: {
                        if (wheel.angleDelta.y < 0)
                            columnTumbler.currentIndex++
                        else
                            columnTumbler.currentIndex--
                        if (placeFilterSwitch.checked == true)
                        {
                            place = (boxTumbler.currentIndex + 1) + "/" + (rowTumbler.currentIndex + 1) + "/" + (columnTumbler.currentIndex + 1)
                            appCore.getDetails(0,0, place)
                        }
                    }
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Номер столбца")//("Column Number")
            }
            //Свитч для быстрого выключения фильтра
            MySwitch {
                width: 75
                height: 75
                id: placeFilterSwitch
                onCheckedChanged: {
                    if (checked == true)
                    {
                        colFilterSwitch.checked = false
                        catFilterSwitch.checked = false
                        place = (boxTumbler.currentIndex + 1) + "/" + (rowTumbler.currentIndex + 1) + "/" + (columnTumbler.currentIndex + 1)
                        appCore.getDetails(0,0, place)
                    }
                    else appCore.getDetails(chooseCatFilterDialog.chosenFilter, colorFilterDlg.chosenFilter, "0/0/0")
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Фильтр Вкл/Выкл")//("Filter On/Off")
            }

            ToolSeparator { }

            ToolButton {
                width: 75
                height: 75
                Image {
                    anchors.fill: parent
                    source: "assets/clr_sizeIconSmall.png"//сюда иконку размера
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Уменьшить размер иконок")//("Smaller size of icons")
            }
            //Слайдер для изменения размера элементов с деталями
            MySlider {
                width: 300
                height: 75
                id: sizeSlider
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Изменить размер иконок")//("Change size of icons")
            }

            ToolButton {
                width: 75
                height: 75
                Image {
                    anchors.fill: parent
                    source: "assets/clr_sizeIcon.png"//сюда иконку размера
                }
                hoverEnabled: true
                ToolTip.delay: 800
                ToolTip.timeout: 4000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Увеличить размер иконок")//("Bigger size of icons")
            }
        }
    }

    Menu { //Подключение БД
        id: connectionMenu
        title: qsTr("База данных")//"Database"
        background: MyItemRectangle {implicitHeight: 50; implicitWidth: 120}
        MenuItem {
            text: qsTr("Открыть соединение")//"Open connection"
            onTriggered: fileDlg.open()
        }
        MenuItem {
            id: item
            text: qsTr("Закрыть соединение")//"Close connection"
            onTriggered: {
                appCore.closeDatabase()
                db_connected = false
                connectionLabel.text = qsTr("Нет соединения")//"No connection"
                statusLabel.text = qsTr("Соединение закрыто")//"Closed connection"
            }
        }
    }

    Menu { //Работа с деталями
        id: editMenu
        title: qsTr("Редактировать...")//"Edit..."
        background: MyItemRectangle {implicitHeight: 50; implicitWidth: 120}
        MenuItem {
            text: qsTr("Детали")//"Detail"
            onTriggered: appCore.openDetailDialog()
        }
        MenuItem {
            text: qsTr("Расположение")//"Placement"
            onTriggered: appCore.openPlacementDialog()
        }
        MenuItem {
            text: qsTr("Категории")//"Category"
            onTriggered: appCore.openSubCategoryDialog()
        }
        MenuItem {
            text: qsTr("Цвета")//"Color"
            onTriggered: appCore.openColorDialog()
        }
    }

    //Статус бар. Слева находится вывод отклика последних действий
    //Справа - указатель на БД с кторой работаем
    footer: ToolBar {
         background: MyHeaderRectangle {}
                id: mainStatusBar
                height: statusLabel.height
                objectName: "myBar"
                    Label {
                        id: statusLabel;
                        text: qsTr("Приложение готово к работе")//"App is ready to work"
                        anchors.left: parent.left
                    }
                    Label {
                        id: connectionLabel;
                        text: qsTr("Нет соединения к базе")//"No connection to database"
                        font.bold: true
                        anchors.right: parent.right
                    }
            }
    //изображение фона при отключенной БД
    Image {
        id: starterImage
        anchors.centerIn: parent
        source: "assets/backgroundPic.png"
        width: parent.height/2
        height: parent.height/2
        visible: !db_connected
    }

    //сетка с деталями, отображаемая при подключении к БД
    ShowDetails {
        id: mainDetails
        visible: db_connected
        iconSize: sizeSlider.value
    }

    //диалог для отображения сообщений
    ShowMessageDialogs {id: messageDialog}
    //диалоги редактирования БД
    CategoryEditDialog {id: categoryDialog}
    ColorEditDialog {id: colorDialog}
    PlacementEditDialog {id: placementDialog}
    DetailEditDialog {
        id: detailDialog
        onClosed: appCore.getDetails()
    }
    //Диалоги фильтров
    ChooseCategoryFilterDialog {
        id: chooseCatFilterDialog
        onClosed: {
            if (chooseCatFilterDialog.chosenFilter != 0)
            {
                catFilterSwitch.checked = true
                appCore.getDetails(chooseCatFilterDialog.chosenFilter, colorFilterDlg.chosenFilter, "0/0/0")
            }
        }        
    }
    ColorFilterDialog {
        id: colorFilterDlg
        onClosed: {
            if (colorFilterDlg.chosenFilter != 0)
            {
                colFilterSwitch.checked = true
                appCore.getDetails(chooseCatFilterDialog.chosenFilter, colorFilterDlg.chosenFilter, "0/0/0")
            }
        }
    }

    DetailInfoDialog {
        id: detailInfoDlg
        onClosed: mainDetails.pressedID = -1
    }
    //Диалог открытия файла для выбора файла с БД
    MyFileDialog {
        id: fileDlg
        nameFilters: ["SQLite database (*.s3db)"]
        onAccepted: {
                console.log(fileDlg.file)
                connectionLabel.text = "Database " + fileDlg.file
                appCore.openDatabase(fileDlg.file)//передаем полученный адрес базы
                db_connected = true
        }
    }
}
