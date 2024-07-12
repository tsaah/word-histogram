#include "topwordstorage.h"

void TopWordStorage::store(const QString &word) {
    const auto count = ++wordCountHash_[word]; // track every word occurance
    if (Q_LIKELY(count <= minimumThreshold_)) { return; } // check if word count is too small so we can early exit
    if (Q_LIKELY(storage_.contains(count - 1))) { // if we had this word with old count value
        auto& oldSet = storage_[count - 1];
        oldSet.erase(word); // remove word with old count value
        if (Q_UNLIKELY(oldSet.empty())) { storage_.erase(count - 1); } // and if set of words became empty - remove the set
    }
    auto& wordSet = storage_[count];
    if (Q_UNLIKELY(wordSet.size() < maxSize_)) { wordSet.emplace(word); } // store word into a set only if set is smaller than max size
    if (Q_LIKELY(storage_.size() > maxSize_)) { storage_.erase(storage_.begin()); } // trim storage size to macSize_
    if (Q_LIKELY(storage_.size() == maxSize_)) { minimumThreshold_ = storage_.begin()->first; } // update threshold
}

std::vector<WordCount> TopWordStorage::getFlatStorage() const {
    std::vector<WordCount> result;
    result.reserve(maxSize_);
    for (auto it = storage_.rbegin(); it != storage_.rend(); ++it) { // go through map in reverse (biggest counts first)
        const auto& [count, wordSet] = *it;
        for (const auto& word: wordSet) { // go through word set
            result.emplace_back<WordCount>({ word, count }); // populate a vector
            if (result.size() == maxSize_) { return result; } // if our vector size is reached - return
        }
    }
    return result;
}
