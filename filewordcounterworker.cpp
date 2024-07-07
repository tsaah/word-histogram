#include "filewordcounterworker.h"

#include <QFile>
#include <QTextStream>
#include <QElapsedTimer>
#include <QDebug>

#include <chrono>
using namespace std::chrono_literals;

FileWordCounterWorker::FileWordCounterWorker(): QObject{} {
    connect(this, &FileWordCounterWorker::startSignal, this, &FileWordCounterWorker::onStart, Qt::QueuedConnection);
}

void FileWordCounterWorker::start(const QUrl& url) {
    // this will be called from main thread so we gonna use queued connection here to  start processing
    // it is expected to be called only when not processing
    emit startSignal(url);
}

void FileWordCounterWorker::pause() {
    // so prior to c++20 I would have gone std::condition_variable or QWaitCondition
    // however c++20 brings in new atiomic functionality that brings very same behavior with less hassle
    paused_ = true; // so in order to pause wi just change this atomic from any thread
}

void FileWordCounterWorker::resume() {
    paused_ = false;
    paused_.notify_one(); // and in order to resume we also going to notify our thread to wake up
}

void FileWordCounterWorker::abort() {
    aborted_ = true;
    resume();
}

void FileWordCounterWorker::onStart(const QUrl &url) {
    QElapsedTimer timer;
    timer.start();

    QFile file(url.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit finished();
        return;
    }

    std::vector<WordCount> snapshot;
    const auto fileSize = file.size();
    QString partialWord;
    QString word;
    partialWord.reserve(128);
    word.reserve(128);
    qint64 pos{ 0 };
    QTextStream stream(&file);
    QChar c;
    while (!stream.atEnd() && !aborted_) {
        if (Q_UNLIKELY(paused_)) { paused_.wait(true); } // here we will suspend our thread if paused until it is notified agamn
        if (Q_UNLIKELY(aborted_)) { break; } // here we are checking if we child interrupt our process after resuming

        // getting a progress value would be easy with stream.pos() but this method well traverse file from very beginning each time we call it and the bigger the file - the worse it gets
        // so i decided to simplify a bit and treat incoming data as utf-8 chars
        // those are 1byte long and allows me to calculate position for progress
        stream >> c;
        ++pos;
        if (Q_UNLIKELY(c.isSpace())) {
            // every time we get space chars we gonna swap word with partialWord effectively clearing partialWord
            word.swap(partialWord);
        } else {
            // until we read space char we will assemble our word in a partialWord
            partialWord.append(c);
        }

        if (Q_UNLIKELY(stream.atEnd())) { word.swap(partialWord); } // don't forget to account for last word if eny

        if (Q_UNLIKELY(word.isEmpty())) { continue; } // howevery do nothing if for some reason our word is empty

        const auto count = ++wordCountHash_[word]; // update our word count hash
        topWordsStorage_.emplaceUnique({ word, count }); // and update our top words priority queue
        word.clear(); // clear word - so it can be reused again

        // now this part
        // our file streaming and word counting is pretty fast and we do not want to overwhelm our model with every single word we got
        // so i decided to emit update signals every so often and ~60 Hz should be close enough to be persieved as "smooth" visually
        // I am using QElapsedTimer to trigger an snapshot creation
        if (Q_UNLIKELY(timer.durationElapsed() > 16ms)) {
            emit progress(pos / static_cast<qreal>(fileSize));

            snapshot = topWordsStorage_.data(); // copy our unsorted top word count data
            std::sort(snapshot.begin(), snapshot.end(), [](const WordCount& a, const WordCount& b){ return a.count > b.count; }); // and sort it

            emit snapshotUpdate(snapshot);
            timer.restart();
        }
    }
    file.close();

    // when we finished or aborted the processing we will emit latest available data and exit so this worker can be removed

    snapshot = topWordsStorage_.data();
    std::sort(snapshot.begin(), snapshot.end(), [](const WordCount& a, const WordCount& b){ return a.count > b.count; });
    emit snapshotUpdate(snapshot);
    emit progress(aborted_ ? 0.0 : 1.0);

    emit finished();
}

