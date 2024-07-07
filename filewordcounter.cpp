#include "filewordcounter.h"

FileWordCounter::FileWordCounter(QObject* parent)
    : QObject{ parent }
{
    // we will start a thread right away only once
    thread_.start();
}

FileWordCounter::~FileWordCounter() {
    // and we will stop our thread at the very end of our application
    abort(); // we also want to interrupt our processing
    thread_.quit();
    thread_.wait();
}

QUrl FileWordCounter::fileUrl() const {
    return fileUrl_;
}

void FileWordCounter::setFileUrl(const QUrl& newFileUrl) {
    // we also only set valid readable local file url
    const auto fileInfo = QFileInfo(newFileUrl.toLocalFile());
    if (!isFileUsable(fileInfo)) {
        return;
    }
    fileUrl_ = newFileUrl;
    emit fileUrlChanged();
}

qreal FileWordCounter::progress() const {
    return progress_;
}

FileWordCounter::FileWordCounterState FileWordCounter::state() const {
    return state_;
}

void FileWordCounter::start() {
    if (fileUrl_.isEmpty()) {
        return;
    }
    if (state_ == Running || state_ == Paused) {
        return;
    }
    if (state_ == Aborting) {
        return;
    }

    setState(Running);
    setProgress(0.0);

    // every time we start processing of a new file - we will create a new worker
    worker_.reset(new FileWordCounterWorker);

    connect(worker_.get(), &FileWordCounterWorker::progress, this, &FileWordCounter::setProgress, Qt::QueuedConnection);
    connect(worker_.get(), &FileWordCounterWorker::snapshotUpdate, this, &FileWordCounter::snapshotUpdate, Qt::QueuedConnection);
    connect(worker_.get(), &FileWordCounterWorker::finished, this, &FileWordCounter::onWorkerFinished, Qt::QueuedConnection);

    worker_->moveToThread(&thread_);

    worker_->start(fileUrl_);
}

void FileWordCounter::pause() {
    if (state_ != Running) {
        return;
    }
    setState(Paused);
    worker_->pause();
}

void FileWordCounter::resume() {
    if (state_ != Paused) {
        return;
    }
    setState(Running);
    worker_->resume();
}

void FileWordCounter::abort() {
    if (state_ != Running && state_ != Paused) {
        return;
    }
    setState(Aborting);
    worker_->abort();
}

void FileWordCounter::reset() {
    if (state_ == Idle) { return; }
    if (state_ == Finished) {
        setFileUrl({});
        setProgress(0.0);
        setState(Idle);
    } else {
        abort();
        shouldReset_ = true;
    }
}

void FileWordCounter::onWorkerFinished() {
    worker_.reset();
    if (shouldReset_) {
        setState(Idle);
        shouldReset_ = false;
    } else {
        setState(Finished);
    }
}

bool FileWordCounter::isFileUsable(const QFileInfo& fileInfo) const {
    return fileInfo.exists() && fileInfo.isFile() && fileInfo.isReadable();
}

void FileWordCounter::setProgress(qreal newProgress) {
    if (qFuzzyCompare(progress_, newProgress)) { return; }
    progress_ = newProgress;
    emit progressChanged();
}

void FileWordCounter::setState(FileWordCounterState newState) {
    if (state_ == newState) { return; }
    state_ = newState;
    emit stateChanged();
}
