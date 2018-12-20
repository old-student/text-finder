import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Web scanner")

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        ControlArea {
            anchors.left: parent.left
            anchors.right: parent.right
        }

        ReportArea {
            anchors.left: parent.left
            anchors.right: parent.right
            Layout.fillHeight: true
        }

        ProgressBar {
            anchors.left: parent.left
            anchors.right: parent.right
            value: scanner.progressValue
        }
    }
}

