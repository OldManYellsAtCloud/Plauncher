import QtQuick

Rectangle {
    property alias img: pic.source
    property alias text: label.text
    property alias pid: pid.text

    width: 45
    height: 70
    Rectangle {
        id: picframe
        width: 45
        height: 60
        border.color: "red"

        Image {
            id: pic
            anchors.centerIn: parent
            width: 40
            height: 40
            clip: false
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                visibilityHandler.hide()
                taskHandler.bringWindowToForeground(pid.text)
            }
        }
    }

    Text {
        id: label
        topPadding: 3
        anchors.top: picframe.bottom
        anchors.horizontalCenter: picframe.horizontalCenter
        font.pixelSize: 15
        height: 10
        color: "black"
    }

    Text {
        id: pid
        anchors.top: picframe.bottom
        anchors.left: picframe.left
        anchors.topMargin: -16
        font.pixelSize: 10
        color: "black"
    }

    Rectangle {
        id: closeButton
        anchors.right: picframe.right
        anchors.top: picframe.top
        height: 20
        width: 20
        radius: 10
        color: "red"
        opacity: 0.5
        Text {
            anchors.centerIn: parent
            text: "X"
            font.pixelSize: 12
            font.bold: true
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    launcherModel.runCommand("kill " + pid.text)
                }
            }
        }
    }
}
