import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import syncmpv 1.0

import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.3 // ApplicationWindow


Rectangle {
    x: 132
    width: 200
    color: "#ffffff"

    property Client client: client
    property Host host: host

    MouseArea {
        id: side_area
        hoverEnabled: true
        anchors.fill: parent

        Button {
            id: load_button
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
                    antialiasing: true
                    font.pointSize: 8
                    anchors.fill: parent
                }
            }

        }

        Button {
            id: host_button
            x: 16
            y: 131
            text: qsTr("Host")
        }

        Button {
            id: client_button
            x: 110
            y: 131
            text: qsTr("Client")
        }

        ListView {
            id: clients_list
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: host_button.bottom
            anchors.topMargin: 30
            anchors.right: parent.right
            anchors.rightMargin: -10
            anchors.left: parent.left
            anchors.leftMargin: 10
            spacing: 16

            delegate: Item {
                height: 40

                ColumnLayout {
                    id: columnLayout
                    height: parent.height

                    Text {
                        text: name
                        antialiasing: true
                        font.pointSize: 8
                    }

                    ProgressBar { // play progress
                        Layout.preferredWidth: side_area.width - 20
                        Layout.preferredHeight: 22
                        value: playProgress
                        maximumValue: 100

                        style: ProgressBarStyle {
                            background: Rectangle {
                                radius: 2
                                color: "lightgray"
                                border.color: "gray"
                                border.width: 1
                                implicitWidth: 200
                                implicitHeight: 24
                            }
                            progress: Rectangle {
                                color: "lightsteelblue"
                                border.color: "steelblue"
                            }
                        }

                        ProgressBar { // buffer progress
                            width: side_area.width - 20 - Math.ceil(parent.value / 100 * parent.width)
                            anchors.left: parent.left
                            anchors.leftMargin: Math.floor(parent.value / 100 * parent.width)
                            height: parent.height
                            value: bufferProgress
                            maximumValue: 100

                            style: ProgressBarStyle {
                                background: Rectangle {
                                    radius: 2
                                    color: "lightgray"
                                    border.color: "gray"
                                    border.width: 1
                                    implicitWidth: 200
                                    implicitHeight: 24
                                }
                                progress: Rectangle {
                                    color: "orange"
                                    border.color: "brown"
                                }
                            }

                        }

                        Text {
                            text: "buffer"
                            style: Text.Normal
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            width: parent.width
                            height: parent.height
                        }
                    }
                }
            }

            model: clientInfoModel
        }

    }

    Connections {
        target: side_area
        onExited: side_menu.opacity = 0
    }

    Connections {
        target: side_area
        onEntered: side_menu.opacity = 1
    }

    Connections {
        target: load_button
        onClicked: mpv.command(["loadfile", textArea.text])
    }

    Connections {
        target: host_button
        onClicked: {
            client_button.enabled = false
            host_button.enabled = false
            host.openConnection()
        }

    }

    Connections {
        target: client_button
        onClicked: {
            host_button.enabled = false
            client_button.enabled = false
            client.connect()
        }

    }
}


