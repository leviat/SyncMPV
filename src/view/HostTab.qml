import QtQuick 2.7
import syncmpv 1.0

import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0

Rectangle {
    height: parent.height
    width: parent.width
    color: "white"

    property Host host: host

    ColumnLayout {
        id: hostColumn
        anchors.fill: parent
        spacing: 5

        Item {
            id: item1
            Layout.minimumHeight: 40
            Layout.fillWidth: true

            Text {
                id: portDescriptor
                text: qsTr("Port:")

                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: portTextFrame.verticalCenter
                font.pixelSize: 12
            }

            Rectangle {
                id: portTextFrame
                border.width: 1

                width: 70
                height: 25
                color: "#ffffff"
                anchors.left: portDescriptor.right
                anchors.leftMargin: 10

                TextField {
                    id: portText
                    placeholderText: "<= 65535"
                    text: "8000"
                    validator: IntValidator {bottom:0; top:65535}

                    anchors.fill: parent
                    antialiasing: true
                    font.pointSize: 8
                }
            }

            Button {
                id: host_button
                width: 70
                height: 20
                text: qsTr("Host")
                highlighted: false
                anchors.left: portTextFrame.right
                anchors.leftMargin: 15
                anchors.verticalCenter: portTextFrame.verticalCenter
            }


        }

        ListView {
            id: clients_list
            spacing: 16
            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: -5

            delegate: Item {
                height: 40

                ColumnLayout {
                    Layout.fillHeight: true
                    width: parent.width

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

    Connections {
        target: host_button
        onClicked: {
            host_button.enabled = false
            host.openConnection(portText.text)
        }

    }
}
