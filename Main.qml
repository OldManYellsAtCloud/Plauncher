import QtQuick
import QtQuick.Controls
import QtQuick.Window

import sgy.pine.bus

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Button {
        id: restartButton
        text: qsTr("Restart")
        font.pixelSize: 18
        font.bold: true
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width / 2
        leftPadding: 5
    }

    Button {
        id: shutdownButton
        text: qsTr("Shut down")
        font.pixelSize: 18
        font.bold: true
        anchors.left: restartButton.right
        anchors.top: parent.top
        width: parent.width / 2
        rightPadding: 5
    }

    Text {
        id: brightnessText
        text: qsTr("Brightness")
        font.bold: true
        font.pixelSize: 20
        anchors.top: shutdownButton.bottom
        anchors.horizontalCenter: slider.horizontalCenter
    }

    Slider {
        id: slider
        from: 0
        to: 100
        width: parent.width
        anchors.top: brightnessText.bottom
    }

    GridView {
        // topPadding: 20
        // leftPadding: 20
        // columns: 4
        // spacing: 15
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: slider.bottom
        anchors.bottom: parent.bottom

        model: launcherModel
        delegate: LauncherDelegate {
            // required property int index
            img: "file:" + model.icon
            text: model.launcherName
        }
    }

    LauncherModel {
        id: launcherModel
    }

}
