import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import Qt.labs.controls 1.0

Item {
    id: root

    Label {
        id: header
        text: qsTr("Report:")
        font.bold: true
    }

    ListView {
        id: listView
        clip: true
        spacing: 2
        anchors.left: parent.left
        anchors.right: parent.right
        y: header.y + header.height + 5
        height: parent.height - header.height - 5

        model: 100
        delegate: Text {
            text: "Text: " + modelData
        }

        ScrollBar.vertical: ScrollBar {
            parent: listView.parent
            active: true
        }
    }
}
