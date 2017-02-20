import QtQuick 2.0

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

    Client {
        id: client
    }

    Host {
        id: host
    }

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
                    font.family: "Arial"
                    antialiasing: false
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
            spacing: 10

            delegate: Item {
                height: 40

                ColumnLayout {
                    id: columnLayout
                    height: parent.height

                    Text {
                        text: name
                        font.bold: true
                    }

                    ProgressBar {
                        Layout.preferredWidth: side_area.width - 20
                        Layout.preferredHeight: 22
                        value: progress
                        maximumValue: 100

                        ProgressBar {
                            width: side_area.width - 20 - Math.ceil(parent.value / 100 * parent.width)
                            anchors.left: parent.left
                            anchors.leftMargin: Math.floor(parent.value / 100 * parent.width)
                            height: parent.height
                            value: 0
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

            model: ListModel {
                ListElement {
                    name: "localhost"
                    progress: 10
                }

                ListElement {
                    name: "Red"
                    progress: 20
                }

                ListElement {
                    name: "Blue"
                    progress: 50
                }

                ListElement {
                    name: "Green"
                    progress: 100
                }
            }

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


