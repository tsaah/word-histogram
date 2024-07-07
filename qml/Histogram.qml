import QtQuick
import QtQuick.Layouts
import my.word_histogram

Rectangle {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.minimumHeight: layout.childrenRect.height
    color: theme.backgroundColor
    border.color: theme.borderColor
    border.width: 1
    required property WordCountModel model

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 1
        spacing: 1
        Repeater {
            model: root.model
            HistogramBar {
                mostUsedWordCount: root.model.mostUsedWordCount
            }
        }
    }
}
