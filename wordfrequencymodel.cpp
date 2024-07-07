#include "wordfrequencymodel.h"

const int WordFrequencyModel::WORD_ROLE = Qt::UserRole + 0;
const int WordFrequencyModel::COUNT_ROLE = Qt::UserRole + 1;

const QHash<int, QByteArray> WordFrequencyModel::ROLES = {
    { WORD_ROLE, "word" },
    { COUNT_ROLE, "count" },
};

WordFrequencyModel::WordFrequencyModel(QObject* parent)
    : QAbstractListModel{ parent }
{
}

int WordFrequencyModel::rowCount(const QModelIndex& parent) const {
    return dataStorage_.size();
}

QVariant WordFrequencyModel::data(const QModelIndex& index, int role) const {
    const auto row = index.row();
    const auto& rowData = dataStorage_[row];
    if (role == WORD_ROLE) {
        return rowData.word;
    } else if (role == COUNT_ROLE) {
        return rowData.count;
    }
    return {};
}

QHash<int, QByteArray> WordFrequencyModel::roleNames() const {
    return ROLES;
}

void WordFrequencyModel::updateData(std::vector<WordCount> snapshot) {
    if (Q_UNLIKELY(snapshot.size() != dataStorage_.size())) {
        // due to our data can only grow when processsing a file, we can add entries just by size difference
        // also this branch will be entered only 15 times when new words are added
        const auto newRows = snapshot.size() - dataStorage_.size();
        beginInsertRows({}, 0, newRows - 1);
        dataStorage_.swap(snapshot);
        endInsertRows();
    } else {
        dataStorage_.swap(snapshot); // in both cases we will swap our old data with incoming snapshot
    }
    emit dataChanged(index(0), index(dataStorage_.size() - 1)); // and signal that every data entry has changed
    setMostUsedWordCount(dataStorage_.front().count); // also our most used word count is updated here
}

FileWordCounter* WordFrequencyModel::fileWordCounter() const {
    return fileWordCounter_;
}

void WordFrequencyModel::setFileWordCounter(FileWordCounter* newFileWordCounter) {
    if (fileWordCounter_ == newFileWordCounter) { return; }
    fileWordCounter_ = newFileWordCounter;
    connect(fileWordCounter_, &FileWordCounter::snapshotUpdate, this, &WordFrequencyModel::updateData, Qt::QueuedConnection);
    emit fileWordCounterChanged();
}

quint64 WordFrequencyModel::mostUsedWordCount() const {
    return mostUsedWordCount_;
}

void WordFrequencyModel::clear() {
    beginResetModel();
    dataStorage_.clear();
    setMostUsedWordCount(0);
    endResetModel();
}

void WordFrequencyModel::setMostUsedWordCount(quint64 newMostUsedWordCount) {
    if (mostUsedWordCount_ == newMostUsedWordCount) { return; }
    mostUsedWordCount_ = newMostUsedWordCount;
    emit mostUsedWordCountChanged();
}
