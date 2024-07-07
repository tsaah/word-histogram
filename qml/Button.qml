import QtQuick

Rectangle {
    id: root
    height: 32
    width: 100
    property bool enabled: true
    required property string text
    required property var action
    color: theme.backgroundColor
    border.color: buttonMouseArea.containsMouse ? theme.accentColor : theme.borderColor
    border.width: 1
    Behavior on border.color { ColorAnimation { duration: theme.animationDuration; easing.type: theme.animationEasingType } }
    Text {
        id: buttonText
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: root.enabled ? theme.textColor : theme.disabledTextColor
        Behavior on color { ColorAnimation { duration: theme.animationDuration; easing.type: theme.animationEasingType } }
        text: root.text
    }
    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        enabled: root.enabled
        hoverEnabled: enabled
        acceptedButtons: Qt.LeftButton
        onClicked: action()
    }
}
