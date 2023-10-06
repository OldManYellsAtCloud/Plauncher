import QtQuick

Rectangle {
    property alias img: pic.source
    property alias text: label.text
    width: 45
    height: 70
    Rectangle {
        id: picframe
        width: 45
        height: 60
        border.color: "red"
        border.width: 1
        Image {
            id: pic
            anchors.centerIn: parent
            width: 40
            height: 40
            clip: false
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

    MouseArea {
        anchors.fill: parent
        onClicked: {
            launcherModel.runCommand(index)
            // hide the launcher so the new app will have the space
            visibilityHandler.hide()
        }
    }
}
