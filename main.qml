import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import Sensor 1.0

ApplicationWindow {
    id:root
    visible: true
    width: 640
    height: 480
    title: qsTr("MODBUS APP")

    menuBar: MenuBar{
        Menu{
            title: qsTr("&File")
            Action {
                text: qsTr("&Baud vs Timer")
                property variant win
                onTriggered: {
                    var component = Qt.createComponent("BaudrayConfig.qml");
                    win = component.createObject(root)
                    root.setVisible(false)
                    win.show();
                }
            }

            Action {
                text: qsTr("&Add New")
                property variant win
                onTriggered: {
                    sensorList.appendItem()
                    var component = Qt.createComponent("Setting.qml");
                    win = component.createObject(root);
                    root.setVisible(false)
                    win.show();
                }
            }

            Action {
                text: qsTr("&Setting")
                property variant win
                onTriggered: {
                    var component = Qt.createComponent("ConFigg.qml");
                    win = component.createObject(root)
                    //                    mymain.push(connfig)
                    root.setVisible(false)
                    win.show();
                }
            }

            Action {
                text: qsTr("&Remove")
                property variant win
                onTriggered: {
                    sensorList.removeCompletedItems()
                    txtdata.text = "Remove sensor"
                }
            }
        }
    }

    Rectangle{
        id:mainn
        anchors.fill: parent

        Rectangle{
            id:grisview
            visible: false
            width: mainn.width
            height: mainn.height - textdata.height

            Component {
                id: appDelegate

                Rectangle {
                    width: 150; height: 150
                    Image {
                        id: myIcon
                        width: parent.width
                        height: parent.height/2 - 10
                        source: "/images/icon.png"
                    }

                    Row{
                        id: rowx1
                        spacing: 15
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            id: data1
                            text: model.data1
                        }

                        Text {
                            id:data2
                            text: model.data2
                        }
                    }


                    Text {
                        id: txtname
                        anchors{
                            bottom: box.top
                            horizontalCenter: parent.horizontalCenter
                        }
                        text: model.description
                    }

                    //                    Button{
                    //                        anchors{
                    //                            bottom: parent.bottom
                    //                            horizontalCenter: parent.horizontalCenter
                    //                        }
                    //                        height: 10
                    //                        width: 10
                    //                        text: ":"
                    //                    }

                    CheckBox{
                        id: box
                        anchors{
                            bottom: parent.bottom
                            horizontalCenter: parent.horizontalCenter
                        }
                        checked: {
                            model.done
                        }

                        onClicked:{
                            model.done = checked
                        }
                    }
                }
            }



            GridView {
                id:gridd
                anchors.fill: parent
                cellWidth: 200; cellHeight: 150
                model: SensorModel{
                    list: sensorList
                }
                focus: true
                delegate: appDelegate
            }
        }
    }



    Row{
        id: butoncontrol
        visible: false
        anchors{
            horizontalCenter: textdata.horizontalCenter
            bottom: textdata.top
        }

        Button{
            text: "Read Data"

            onClicked: {
                sensorList.myReadData(0)
            }
        }

        Button{
            text: "Read ID"

            onClicked: {
                sensorList.myReadData(100)
            }
        }

        Button{
            text: "Auto Read"

            onClicked: {
                sensorList.setautoread()
                butoncontrol.visible = false
            }
        }
    }


    Row{
        id: textdata
        anchors.right: mainn.right
        anchors.bottom: mainn.bottom

        Rectangle {
            border.width: 2
            width: mainn.width - buttContro.width
            height: buttContro.height
            Text {
                id: txtdata
                anchors.centerIn: parent
                text: qsTr("text")
            }
        }
        Column{
            id: buttContro
            spacing: 2
            Button{
                text: "Connect"

                onClicked: {
                    sensorList.myConnect()
                    butoncontrol.visible = true
                    grisview.visible = true
                    txtdata.text = "Connect OK"
                }

            }

            Button{
                text: "Quit"

                onClicked: {
                    sensorList.returndata()
                    butoncontrol.visible = false
                    txtdata.text = "End Connect"
                    sensorList.endConnection()
                    sensorList.showdata()
                }
            }
        }
    }
}






