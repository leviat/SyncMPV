import QtQuick 2.7
import QtQuick.Controls.Styles 1.4
import syncmpv 1.0

import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Controls 2.0


Rectangle {
    id: sidebar
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

            //ToolSeparator {
            //    y: 0
            //    Layout.fillWidth: true
            //    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            //    orientation: Qt.Horizontal
            //}

            Item {
                id: modeArea
                Layout.fillWidth: true
                Layout.maximumHeight: 30
                Layout.minimumHeight: 20

                Button {
                    id: clientModeButton
                    width: 70
                    height: 20
                    text: qsTr("Client mode")
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                }

                Button {
                    id: hostModeButton
                    width: 70
                    height: 20
                    text: qsTr("Host mode")
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                }
            }

            Item {
                id: hostClientTabArea
                Layout.fillWidth: true
                Layout.fillHeight: true

                HostTab {
                    id: hostTab
                    host: sidebar.host
                    enabled: false
                    visible: false
                }

                ClientTab {
                    id: clientTab
                    client: sidebar.client
                    enabled: false
                    visible: false
                }
            }

            Item {
                Layout.fillHeight: true
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
        target: hostModeButton
        onClicked: {
            modeArea.enabled = false
            modeArea.visible = false
            hostTab.enabled = true
            hostTab.visible = true
        }
    }

    Connections {
        target: clientModeButton
        onClicked: {
            modeArea.enabled = false
            modeArea.visible = false
            clientTab.enabled = true
            clientTab.visible = true
        }
    }

}


