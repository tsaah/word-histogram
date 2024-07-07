import QtQuick
import QtQuick.Layouts

Item {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.minimumHeight: barLabel.contentHeight + 4
    required property int count
    required property string word
    required property int mostUsedWordCount
    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * count / mostUsedWordCount
        height: parent.height
        color: theme.accentColor
        border.color: theme.borderColor
        border.width: 1
        Behavior on width { NumberAnimation { duration: theme.animationDuration; easing.type: theme.animationEasingType } }
        Text {
            id: barLabel
            anchors.fill: parent
            visible: contentWidth + 5 < width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: theme.textColor
            font.pointSize: 12
            text: '<b>' + root.word + '</b> (' + String(root.count) + ')'
        }
    }
}
