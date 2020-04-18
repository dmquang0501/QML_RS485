import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.3

import Sensor 1.0

ApplicationWindow {
    id: window1
    visible: true
    x: 0
    y: 0
    width: 480
    height: 272
    title: qsTr("Setting")

    Rectangle{
        id:settingg
        anchors.fill: parent
        Column{
            anchors.centerIn: parent
            spacing: 5
            Row{
                spacing: 3
                Label {
                    text: "BaudRay"
                }
                TextField {
                    width: 70
                    height: 30
                    onEditingFinished: sensorList.setBaudray(text);
                }
            }

            Row{
                spacing: 3
                Label {
                    text: "Timer"
                }
                TextField {
                    width: 70
                    height: 30
                    onEditingFinished: sensorList.setTimer(text);
                }
            }
        }

        Row{
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                text: "OK"
                onClicked: {
                    root.show()
                    close()
                }
            }
        }
    }
}
