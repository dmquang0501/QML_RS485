import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.3

import Sensor 1.0

ApplicationWindow {
    id: window2
    visible: true
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
                    text: "slave id"
                }
                TextField {
                    id:txt_slaveid
                    width: 70
                    height: 30
                    onEditingFinished: sensorList.setSlave_id(text);
                }
            }
        }

        Button{
            text: "Connections"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            onClicked: {
                sensorList.returndata
                txtdata.text = "Add New Device OK"
                root.show()
                close()
            }
        }
    }
}
