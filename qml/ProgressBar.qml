import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    Layout.fillWidth: true
    height: 18
    color: theme.backgroundColor
    border.color: theme.borderColor
    border.width: 1
    property real progress: 0
    Rectangle {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 1
        color: theme.accentColor
        width: (parent.width - 2) * root.progress
        Behavior on width { NumberAnimation { duration: theme.animationDuration; easing.type: theme.animationEasingType } }
        Text {
            anchors.fill: parent
            clip: true
            visible: contentWidth + 10 < width
            text: (progressBar.progress * 100).toFixed(0) + ' %'
            color: theme.textColor
            font.pointSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
