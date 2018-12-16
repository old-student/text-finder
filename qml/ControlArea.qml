import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Row {
    id: root

    GridLayout {
        width: parent.width/4*3
        columns: 2

        Label {
            text: qsTr("Input search parameters:")
            font.bold: true
        }
        Label { /* empty placeholder */}

        Label { text: qsTr("starting URL") }
        TextField {
            Layout.fillWidth: true
            text: qsTr("www.google.com.ua")
        }

        Label { text: qsTr("number of threads for scanning") }
        TextField {
            Layout.fillWidth: true
            text: qsTr("1")
        }

        Label { text: qsTr("searching text") }
        TextField {
            Layout.fillWidth: true
            text: qsTr("some text")
        }

        Label { text: qsTr("max number of scanned url's") }
        TextField {
            Layout.fillWidth: true
            text: qsTr("100")
        }
    }

    Item {
        width: parent.width/4
        height: parent.height

        Column {
            anchors.centerIn: parent
            spacing: 10

            Button {
                text: qsTr("Start")
            }
            Button {
                text: qsTr("Stop")
                enabled: false
            }
            Button {
                text: qsTr("Pause")
                enabled: false
            }
        }
    }
}
