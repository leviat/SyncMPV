import QtQuick 2.5

import syncmpv 1.0

import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Controls 2.1

ApplicationWindow {
    id: window
    title: "Leo of the Storm"
    width: 1280
    height: 720
    visible: true

    Item {
        Client {
            id: client
            objectName: "client"
        }

        Host {
            id: host
            objectName: "host"
        }
    }

    MpvObject {
        id: mpv
        objectName: "mpv"
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

    Sidebar {
        id: side_menu
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        client: client
        host: host
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
                    id: playProgress
                    width: 200
                    from: 0
                    to: 100
                    visible: true
                    Layout.minimumWidth: 50
                    Layout.fillWidth: true
                    value: mpv.percentPlaytime

                    Slider {
                        id: timeSlider
                        anchors.rightMargin: -10
                        anchors.leftMargin: -10
                        stepSize: 0.1
                        from: 0
                        to: 100
                        opacity: 0
                        anchors.fill: parent
                    }

                    ProgressBar { // buffer progress
                        width: parent.width - Math.ceil(parent.visualPosition * parent.width)
                        anchors.left: parent.left
                        anchors.leftMargin: Math.floor(parent.visualPosition * parent.width)
                        height: parent.height
                        value: mpv.bufferProgress
                        from: 0
                        to: 100

                        background: Rectangle {
                            color: "#e6e6e6"
                        }

                        contentItem: Item {
                            Rectangle {
                                width: parent.parent.visualPosition * parent.width
                                height: parent.height
                                color: "orange"
                            }
                        }

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
                from: 0
                to: 100
                value: mpv.volume

                handle: Rectangle {
                    x: volume_slider.leftPadding + volume_slider.visualPosition * (volume_slider.availableWidth - width)
                    y: volume_slider.topPadding + volume_slider.availableHeight / 2 - height / 2
                    implicitWidth: 16
                    implicitHeight: 16
                    radius: 8
                    color: volume_slider.pressed ? "#f0f0f0" : "#f6f6f6"
                    border.color: "#bdbebf"
                }

                background: Rectangle {
                    x: volume_slider.leftPadding
                    y: volume_slider.topPadding + volume_slider.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 4
                    width: volume_slider.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "#bdbebf"

                    Rectangle {
                        width: volume_slider.visualPosition * parent.width
                        height: parent.height
                        color: "#21be2b"
                        radius: 2
                    }
                }

                Text {
                    id: volume_text
                    y: 4
                    text: volume_slider.value.toFixed(0) + "\%"
                    anchors.left: parent.right
                    anchors.leftMargin: 10
                    font.pixelSize: 12
                }
            }

            Button {
                id: pauseButton
                x: 273
                y: 34
                text: qsTr("Pause")
                checked: mpv.paused

            }
        }

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

    Connections {
        target: window
        onClosing: {
            host.closeConnection()
            client.disconnect()
        }
    }

    Connections {
        target: pauseButton
        onClicked: mpv.switchPause(true)
    }

}
