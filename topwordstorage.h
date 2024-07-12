#pragma once

#include "wordcount.h"

#include <map>
#include <vector>
#include <unordered_set>

#include <QHash>

class TopWordStorage final {
public:
    void store(const QString& word);
    std::vector<WordCount> getFlatStorage() const;
private:
    QHash<QString, quint64> wordCountHash_;
    std::map<quint64, std::unordered_set<QString>> storage_;
    quint64 minimumThreshold_{ 0 }; // this variable will track an early cutoff threshold in order to skip processing low-count words
    const size_t maxSize_{ 15 };
};
