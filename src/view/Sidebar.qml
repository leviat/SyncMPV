import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import syncmpv 1.0

import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Controls 2.1


Rectangle {
    width: 200
    color: "#ffffff"

    property Client client: client
    property Host host: host

    MouseArea {
        id: side_area
        hoverEnabled: true
        anchors.fill: parent

        ColumnLayout {
            id: mainColumn
            anchors.fill: parent

            spacing: 5

            Item {
                id: fileLoader
                width: 200
                height: 50
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.fillWidth: true
                Layout.preferredHeight: 70
                Layout.preferredWidth: 200
                Layout.minimumWidth: 180
                Layout.minimumHeight: 70

                Rectangle {
                    id: textframe
                    width: 180
                    height: 25
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    border.width: 1

                    TextField {
                        id: textArea
                        anchors.fill: parent
                        placeholderText: "URL / File path"
                        antialiasing: true
                        font.pointSize: 8
                    }
                }

                Button {
                    id: load_button
                    x: 0
                    y: 0
                    width: 60
                    height: 20
                    text: qsTr("Load")
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 45

                }
            }

            ToolSeparator {
                y: 0
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                orientation: Qt.Horizontal
            }

            Item {
                id: hostClientArea
                y: 0
                Layout.fillHeight: true
                Layout.fillWidth: true

                Button {
                    id: client_button
                    width: 70
                    height: 20
                    text: qsTr("Client")
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                }

                Button {
                    id: host_button
                    x: 0
                    y: 0
                    width: 70
                    height: 20
                    text: qsTr("Host")
                    highlighted: false
                    anchors.left: parent.left
                    anchors.leftMargin: 15
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
                                Layout.preferredHeight: 20
                                value: playProgress
                                from: 0
                                to: 100

                                contentItem: Item {

                                    Rectangle {
                                        width: parent.parent.visualPosition * parent.width
                                        height: parent.height
                                        color: "#17a81a"

                                    }
                                }

                                ProgressBar { // buffer progress
                                    width: side_area.width - 20 - Math.ceil(parent.value / 100 * parent.width)
                                    anchors.left: parent.left
                                    anchors.leftMargin: Math.floor(parent.value / 100 * parent.width)
                                    height: parent.height
                                    value: bufferProgress
                                    from: 0
                                    to: 100

                                    background: Rectangle {
                                        implicitWidth: 200
                                        implicitHeight: 6
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

                                Text {
                                    text: bufferString
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


