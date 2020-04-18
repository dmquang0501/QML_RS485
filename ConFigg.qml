import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.3
import Sensor 1.0

ApplicationWindow {
    id: window1
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
                    text: "slave New id"
                }
                TextField {
                    id:txt_newid
                    width: 70
                    height: 30
                    onEditingFinished: sensorList.setnew_id(text);
                }
            }
        }

        Row{
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                text: "Changed ID"
                onClicked: {
                    sensorList.myWriteData()
                    sensorList.myReadData(100)
                    close()
                }
            }
            Button{
                text: "Canced"
                onClicked: {
                    txtdata.text = "Changed OK"
                    root.show()
                    close()
                }
            }
        }
    }
}
