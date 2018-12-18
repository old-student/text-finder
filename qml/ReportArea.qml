import QtQuick 2.6
import QtQuick.Controls 1.4

Item {
    id: root

    Label {
        id: header
        text: qsTr("Report:")
        font.bold: true
    }

    TableView {
        anchors.fill: parent
        model: scanner.reportModel

        TableViewColumn {
            title: "Url"
            role: "url"
            width: parent.width / 2
        }

        TableViewColumn {
            title: "Status"
            role: "status"
            width: parent.width / 2
        }
    }
}
