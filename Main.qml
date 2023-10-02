import QtQuick
import QtQuick.Controls
import QtQuick.Window

import QtCore

import sgy.pine.launcher

Window {
    width: 640
    height: 480
    visible: visibilityHandler.visible
    title: qsTr("Hello World")

    VisiblityHandler {
        id: visibilityHandler
    }

    BrightnessHandler {
        id: brightnessHandler
    }

    ShutdownHandler {
        id: shutdownHandler
    }

    Dialog {
        id: shutdownDialog
        title: qsTr("Shut down?")
        standardButtons: Dialog.Yes | Dialog.Cancel
        onAccepted: {
            shutdownHandler.shutdown()
        }
        anchors.centerIn: parent
    }

    Button {
        id: shutdownButton
        text: qsTr("Shut down")
        font.pixelSize: 18
        font.bold: true
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width / 2
        onClicked: {
            shutdownDialog.open()
        }
    }

    Button {
        id: hideButton
        text: qsTr("Hide")
        font.pixelSize: 18
        font.bold: true
        anchors.left: shutdownButton.right
        anchors.top: parent.top
        width: parent.width / 2
        onClicked: {
            visibilityHandler.hide()
        }
    }

    Text {
        id: brightnessText
        text: qsTr("Brightness: " + slider.value)
        font.bold: true
        font.pixelSize: 20
        anchors.top: shutdownButton.bottom
        anchors.horizontalCenter: slider.horizontalCenter
    }

    Slider {
        id: slider
        from: 0
        to: 100
        stepSize: 1
        width: parent.width
        anchors.top: brightnessText.bottom
        bottomPadding: 5
        onValueChanged: {
            brightnessHandler.brightness = slider.value
        }

        Settings {
            property alias value: slider.value
        }

    }

    GridView {
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
