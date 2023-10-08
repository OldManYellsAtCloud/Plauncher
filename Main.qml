import QtQuick
import QtQuick.Controls
import QtQuick.Window

import QtCore

import sgy.pine.launcher

Window {
    width: 640
    height: 480
    visible: visibilityHandler.visible

    onVisibilityChanged: {
        if (visible){
            taskHandler.hideActiveWindow()
        }
    }

    VisiblityHandler {
        id: visibilityHandler
    }

    BrightnessHandler {
        id: brightnessHandler
    }

    TaskHandler {
        id: taskHandler
    }


    Dialog {
        id: shutdownDialog
        title: qsTr("Shut down?")
        standardButtons: Dialog.Yes | Dialog.Cancel
        onAccepted: {
            launcherModel.runCommand("shutdown -h now")
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
        text: qsTr("Brightness: %1".arg(slider.value))
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

        Component.onCompleted: {
            slider.value = brightnessHandler.brightness
        }

        Settings {
            property alias value: slider.value
        }

    }

    GridView {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.top: slider.bottom
        anchors.bottom: taskmanagerList.top

        cellWidth: (parent.width / 4) - 5

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

    ListView {
        id: taskmanagerList
        orientation: ListView.Horizontal
        model: taskHandler

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        delegate: LauncherDelegate {
            labelVisible: false
        }

    }

}
