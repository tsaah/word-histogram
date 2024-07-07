import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import my.word_histogram

Window {
    id: root
    width: minimumWidth
    height: minimumHeight
    color: '#333'
    visible: true
    title: qsTr('Word Histogram')
    minimumWidth: 1000
    minimumHeight: 500

    readonly property QtObject theme: QtObject {
        property color backgroundColor: '#444'
        property color borderColor: '#555'
        property color textColor: '#eee'
        property color disabledTextColor: '#777'
        property color accentColor: '#95e'
        property int animationDuration: 100
        property int animationEasingType: Easing.Linear
    }

    function openFile(file) {
        fileWordCounter.fileUrl = file
    }

    FileDialog {
        id: fileDialog
        modality: Qt.ApplicationModal
        acceptLabel: qsTr('Open file')
        title: qsTr('Open file for word histogram evaluation')
        onAccepted: root.openFile(currentFile)
    }

    WordCountModel {
        id: wordCountModel
        fileWordCounter: FileWordCounter {
            id: fileWordCounter
            readonly property int isIdle: fileWordCounter.state === FileWordCounter.Idle
            readonly property int isRunning: fileWordCounter.state === FileWordCounter.Running
            readonly property int isPaused: fileWordCounter.state === FileWordCounter.Paused
            readonly property int isFinised: fileWordCounter.state === FileWordCounter.Finished
            onFileUrlChanged: function (url) { start(url) }
        }
    }

    Item {
        id: startPage
        anchors.fill: parent
        visible: fileWordCounter.isIdle

        DropArea {
            anchors.fill: parent
            onDropped: function(drop) {
                if (drop.hasUrls && drop.urls.length === 1) {
                    root.openFile(drop.urls[0])
                } else {
                    root.openFile('')
                }
            }
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 10
            Text {
                Layout.fillWidth: true
                color: theme.textColor
                font.pointSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr('Drop or open a file...')
            }
            Button {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr('Open file ...')
                action: function() { fileDialog.open() }
            }
        }
    }

    ColumnLayout {
        id: histogramPage
        visible: !fileWordCounter.isIdle
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        Histogram {
            id: histogram
            model: wordCountModel
        }
        ProgressBar {
            id: progressBar
            progress: fileWordCounter.progress
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 5
            Item { Layout.fillWidth: true }
            Button {
                property bool running: true
                enabled: fileWordCounter.isRunning || fileWordCounter.isPaused
                text: fileWordCounter.isPaused ? qsTr('Resume') : qsTr('Pause')
                action: function() {
                    if (fileWordCounter.isRunning) {
                        fileWordCounter.pause();
                    } else {
                        fileWordCounter.resume();
                    }
                }
            }
            Item { Layout.fillWidth: true }
            Button {
                text: (fileWordCounter.isRunning || fileWordCounter.isPaused) ? qsTr('Stop') : qsTr('Back')
                action: function() {
                    if (fileWordCounter.isRunning || fileWordCounter.isPaused) {
                        fileWordCounter.abort();
                    }
                    fileWordCounter.reset();
                    wordCountModel.clear();
                }
            }
            Item { Layout.fillWidth: true }
        }
    }
}

