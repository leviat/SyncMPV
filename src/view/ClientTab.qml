import QtQuick 2.0
import syncmpv 1.0
import QtQuick.Controls 2.1

Rectangle {

    property Client client: client

    width: parent.width
    color: "white"

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

    Connections {
        target: client_button
        onClicked: {
            client_button.enabled = false
            client.connect()
        }

    }
}
