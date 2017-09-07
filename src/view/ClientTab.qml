import QtQuick 2.7
import syncmpv 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: rectangle

    property Client client: client

    width: parent.width
    height: parent.height
    color: "white"

    ColumnLayout {
        id: clientColumn
        anchors.fill: parent
        spacing: 5

        Item {
            id: clientNameRow
            Layout.preferredHeight: clientNameTextFrame.height
            Layout.fillWidth: true

            Text {
                id: clientNameDescriptor
                text: qsTr("Client name:")

                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: clientNameTextFrame.verticalCenter
                font.pixelSize: 12
            }

            Rectangle {
                id: clientNameTextFrame
                border.width: 1

                width: 70
                height: 25
                color: "#ffffff"
                anchors.left: clientNameDescriptor.right
                anchors.leftMargin: 5

                TextField {
                    id: clientNameText
                    placeholderText: "ClientX"
                    text: ""

                    anchors.fill: parent
                    antialiasing: true
                    font.pointSize: 8
                }
            }

        }


        Item {
            id: clientPortRow
            Layout.preferredHeight: clientPortTextFrame.height
            Layout.fillWidth: true

            Text {
                id: clientPortDescriptor
                text: qsTr("Client port:")

                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: clientPortTextFrame.verticalCenter
                font.pixelSize: 12
            }

            Rectangle {
                id: clientPortTextFrame
                border.width: 1

                width: 70
                height: 25
                color: "#ffffff"
                anchors.left: clientPortDescriptor.right
                anchors.leftMargin: 5

                TextField {
                    id: clientPortText
                    placeholderText: "<= 65535"
                    text: "8001"
                    validator: IntValidator {bottom:0; top:65535}

                    anchors.fill: parent
                    antialiasing: true
                    font.pointSize: 8
                }
            }

        }

        Item {
            id: hostPortRow
            Layout.fillWidth: true
            Layout.preferredHeight: hostPortTextFrame.height

            Text {
                id: hostPortDescriptor
                text: qsTr("Host port:")

                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: hostPortTextFrame.verticalCenter
                font.pixelSize: 12
            }

            Rectangle {
                id: hostPortTextFrame
                border.width: 1

                width: 70
                height: 25
                color: "#ffffff"
                anchors.left: hostPortDescriptor.right
                anchors.leftMargin: 5

                TextField {
                    id: hostPortText
                    placeholderText: "<= 65535"
                    text: "8000"
                    validator: IntValidator {bottom:0; top:65535}

                    anchors.fill: parent
                    antialiasing: true
                    font.pointSize: 8
                }
            }

        }

        Item {
            id: hostIPRow
            Layout.fillWidth: true
            Layout.preferredHeight: hostIPTextFrame.height

            Text {
                id: hostIPDescriptor
                text: qsTr("Host IP:")

                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: hostIPTextFrame.verticalCenter
                font.pixelSize: 12
            }

            Rectangle {
                id: hostIPTextFrame
                border.width: 1

                width: 100
                height: 25
                color: "#ffffff"
                anchors.left: hostIPDescriptor.right
                anchors.leftMargin: 5

                TextField {
                    id: hostIPText
                    placeholderText: "123.123.123.123"
                    validator: RegExpValidator {
                        regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }

                    anchors.fill: parent
                    antialiasing: true
                    font.pointSize: 8
                }
            }
        }

        Button {
            id: client_button
            text: qsTr("Connect")

            Layout.preferredWidth: 70
            Layout.preferredHeight: 20
            anchors.left: parent.left
            anchors.leftMargin: 5
        }

        Item {
            Layout.fillHeight: true
        }

    }


    Connections {
        target: client_button
        onClicked: {
            client_button.enabled = false
            client.connect(hostIPText.text, hostPortText.text, clientPortText.text, clientNameText.text)
        }

    }

}
