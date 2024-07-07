#pragma once

#include <queue>

#include "wordcount.h"

using BasePriorityQueue = std::priority_queue<WordCount, std::vector<WordCount>, bool(*)(const WordCount&, const WordCount&)>;

// this is a modified priority queue that will allow only unique words and also will keep it's maximum entry count to a maxSize
class UniquePriorityQueue final: public BasePriorityQueue {
public:
    UniquePriorityQueue(size_t maxSize = 15);

    // this method either emplases or updates unique word count
    void emplaceUnique(const WordCount& value);

    // this method gives readonly access to an underlying storage
    const std::vector<WordCount>& data() const;

private:
    static bool compareFunction(const WordCount& a, const WordCount& b);;
    quint64 minimumThreshold_{ 0 }; // this variable will track an early cutoff threshold in order to skip processing low-count words
    const size_t maxSize_{ 0 };
};
