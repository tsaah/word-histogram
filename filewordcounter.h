#pragma once

#include <QFileInfo>
#include <QObject>
#include <QThread>
#include <QtQml/qqmlregistration.h>

#include "filewordcounterworker.h"

// this is a class that will provide an interface to our threaded worker and controll it's behavior
class FileWordCounter: public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(FileWordCounter) // in Qt6 this macro will register this class as a QML class in a module defined in a cmake
    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged FINAL) // this property will hold a local file url
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged FINAL) // this property is keeping a progress in order to bind to it
    Q_PROPERTY(FileWordCounterState state READ state WRITE setState NOTIFY stateChanged FINAL) // this property reflects current state
public:
    enum FileWordCounterState {
        Idle,
        Running,
        Paused,
        Aborting,
        Finished
    };
    // we also using Q_ENUM macro so we can use it in QML
    Q_ENUM(FileWordCounterState)

    explicit FileWordCounter(QObject* parent = nullptr);
    ~FileWordCounter() override;

    QUrl fileUrl() const;
    void setFileUrl(const QUrl& newFileUrl);
    qreal progress() const;
    FileWordCounterState state() const;

public slots:
    // this is main interface which allows us to controll worker object
    // those are fast and cheap methods that are colling worker object's respective methods, and they are designed to be thread safe and non blocking
    void start();
    void pause();
    void resume();
    void abort();
    void reset();

signals:
    void fileUrlChanged();
    void progressChanged();
    void stateChanged();
    void snapshotUpdate(const std::vector<WordCount>& snapshot);

private slots:
    void onWorkerFinished();

private:
    bool isFileUsable(const QFileInfo& fileInfo) const;
    void setProgress(qreal newProgress);
    void setState(FileWordCounterState newState);

    QUrl fileUrl_;
    QThread thread_;
    QScopedPointer<FileWordCounterWorker> worker_;
    qreal progress_{ 0.0 };
    FileWordCounterState state_{ Idle };
    bool shouldReset_{ false };
};
