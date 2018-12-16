import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import scanner 1.0

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
            id: startUrl
            Layout.fillWidth: true
            text: qsTr("www.google.com.ua")            
        }
        Binding { target: scanner; property: "startUrl"; value: startUrl.text }

        Label { text: qsTr("number of threads for scanning") }
        TextField {
            id: threadsNumber
            Layout.fillWidth: true            
            validator: IntValidator { bottom: 1; top: 100 }
            text: qsTr("1")
            placeholderText: qsTr("[1:100]")
        }
        Binding { target: scanner; property: "threadsNumber"; value: threadsNumber.text }

        Label { text: qsTr("searching text") }
        TextField {
            id: searchText
            Layout.fillWidth: true
            text: qsTr("some text")
        }
        Binding { target: scanner; property: "searchText"; value: searchText.text }

        Label { text: qsTr("max number of scanned url's") }
        TextField {
            id: urlLimit
            Layout.fillWidth: true
            validator: IntValidator { bottom: 1; top: 1000000 }
            text: qsTr("1000")
            placeholderText: qsTr("[1:1000000]")
        }
        Binding { target: scanner; property: "urlLimit"; value: urlLimit.text }
    }

    Item {
        width: parent.width/4
        height: parent.height

        Column {
            anchors.centerIn: parent
            spacing: 10

            Button {
                text: qsTr("Start")
                enabled: scanner.status === Scanner.NotRunning;
                onClicked: { scanner.start() }
            }
            Button {
                text: qsTr("Stop")
                enabled: scanner.status === Scanner.Running;
                onClicked: { scanner.stop() }
            }
            Button {
                text: qsTr("Pause")
                enabled: scanner.status === Scanner.Running;
                onClicked: { scanner.pause() }
            }
        }
    }
}
