import QtQuick 2.0
import QtQuick.Controls 1.0

import syncmpv 1.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.3 // ApplicationWindow

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    visible: true

    MpvObject {
        id: mpv
        anchors.fill: parent
        scale: 1

        MouseArea {
            id: screen_area
            anchors.fill: parent
            property real prevMousePosX: 0
            property real prevMousePosY: 0
            property real mouseIdleTime: 0
            hoverEnabled: true
        }
    }

    Timer {
        id: timer
        interval: 200
        repeat: true
        running: true
    }

    Rectangle {
        id: side_menu
        x: 132
        width: 200
        color: "#ffffff"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        MouseArea {
            id: side_area
            hoverEnabled: true
            anchors.fill: parent

            Button {
                id: button
                x: 62
                y: 10
                text: qsTr("Load")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 10

                Rectangle {
                    id: textframe
                    x: -52
                    y: 63
                    width: 180
                    height: 50
                    color: "#ffffff"
                    border.width: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.bottom
                    anchors.topMargin: 10

                    TextArea {
                        id: textArea
                        text: "https://www.youtube.com/watch?v=2F8HvKqnp5s"
                        font.family: "Arial"
                        antialiasing: false
                        font.pointSize: 8
                        anchors.fill: parent
                    }
                }

            }
        }
    }

    Rectangle {
        id: control_box
        x: 118
        y: 86
        width: 800
        height: 80
        color: "#ffffff"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter

        MouseArea {
            id: control_hover
            anchors.fill: parent
            hoverEnabled: true
            enabled: true

            RowLayout {
                id: rowLayout
                x: 5
                y: 5
                height: 20
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5

                ProgressBar {
                    id: progressBar
                    width: 200
                    maximumValue: 100
                    visible: true
                    Layout.minimumWidth: 50
                    Layout.fillWidth: true
                    value: mpv.percentPlaytime

                    Slider {
                        id: timeSlider
                        anchors.rightMargin: -10
                        anchors.leftMargin: -10
                        stepSize: 0.1
                        maximumValue: 100
                        opacity: 0
                        anchors.fill: parent
                    }
                }

                Text {
                    id: timeDisplay
                    width: 60
                    text: mpv.playtime
                    fontSizeMode: Text.VerticalFit
                    font.pixelSize: 12
                    Layout.preferredWidth: 60

                }


            }

            Slider {
                id: volume_slider
                y: 44
                width: 100
                height: 22
                anchors.left: parent.left
                anchors.leftMargin: 5
                stepSize: 1
                updateValueWhileDragging: true
                maximumValue: 100
                value: mpv.volume

                Text {
                    id: volume_text
                    y: 4
                    text: volume_slider.value.toFixed(0) + "\%"
                    anchors.left: parent.right
                    anchors.leftMargin: 10
                    font.pixelSize: 12
                }
            }
        }

    }

    Connections {
        target: button
        onClicked: mpv.command(["loadfile", textArea.text])
    }

    Connections {
        target: control_hover
        onExited: control_box.opacity = 0
    }

    Connections {
        target: control_hover
        onEntered: control_box.opacity = 1
    }

    Connections {
        target: timeSlider
        onValueChanged: mpv.command(["seek", timeSlider.value, "absolute-percent"])
    }

    Connections {
        target: volume_slider
        onValueChanged: {
            mpv.setProperty("ao-volume", volume_slider.value)
        }
    }

    Connections {
        target: screen_area
        onDoubleClicked: if (window.visibility != 5) {window.showFullScreen()} else {window.showNormal()}
    }

    Connections {
        target: side_area
        onExited: side_menu.opacity = 0
    }

    Connections {
        target: side_area
        onEntered: side_menu.opacity = 1
    }

    Connections{
        target: timer
        onTriggered: {
            if(screen_area.containsMouse && screen_area.prevMousePosX == screen_area.mouseX && screen_area.prevMousePosY == screen_area.mouseY) {
                if(screen_area.mouseIdleTime < 1000) {
                    screen_area.mouseIdleTime += timer.interval
                }
                else {
                    screen_area.cursorShape = Qt.BlankCursor
                }
            }

            screen_area.prevMousePosX = screen_area.mouseX
            screen_area.prevMousePosY = screen_area.mouseY
        }
    }

    Connections{
        target: screen_area
        onPositionChanged: {
            screen_area.mouseIdleTime = 0
            screen_area.cursorShape = Qt.ArrowCursor
        }
    }



}
