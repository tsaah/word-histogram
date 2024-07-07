#pragma once

#include <QMutex>
#include <QObject>
#include <QUrl>
#include <QTimer>
#include <QHash>

#include "uniquepriorityqueue.h"

// and this is the worker that will process our file in a separate thread
class FileWordCounterWorker final: public QObject {
    Q_OBJECT
public:
    FileWordCounterWorker();

public slots:
    // all those methods are designed in such a way that it is safe to call them from any thread
    void start(const QUrl& url);
    void pause();
    void resume();
    void abort();

signals:
    void startSignal(const QUrl& url);
    void finished();
    void snapshotUpdate(const std::vector<WordCount>& snapshot);
    void progress(qreal progress);

private slots:
    void onStart(const QUrl& url);

private:
    std::atomic<bool> hasFile_{ false };
    std::atomic<bool> paused_{ false };
    std::atomic<bool> aborted_{ false };

    QHash<QString, quint64> wordCountHash_; // this is a hash that contains all word counts
    UniquePriorityQueue topWordsStorage_; // this is a top 15 word count priority queue
};
