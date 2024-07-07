#include "uniquepriorityqueue.h"

UniquePriorityQueue::UniquePriorityQueue(size_t maxSize)
    : BasePriorityQueue{ compareFunction }
    , maxSize_{ maxSize }
{
    c.reserve(maxSize + 1);
}

void UniquePriorityQueue::emplaceUnique(const WordCount &value) {
    // here we skip processing any wordds with count less than a minimum threshold
    if (value.count < minimumThreshold_) { return; }

    // then we are finding an element containing same word (this is O(k) at most where k is maxSize and is pretty cheap)
    auto it = std::find_if(c.begin(), c.end(), [&value](const WordCount& v){ return value.word == v.word; });

    if (it == c.end()) {
        // in case when it is a new word we will emplace it (this whould be O(log(k)))
        emplace(value);
    } else {
        // otherwise we will update an existing word count and remake a heap which should be linear ~ O(k)
        it->count = value.count;
        std::make_heap(c.begin(), c.end(), comp);
    }
    // here we controll aour queue size
    while (size() > maxSize_) { pop(); }

    // we also update our threshold value
    minimumThreshold_ = top().count;
}

const std::vector<WordCount> &UniquePriorityQueue::data() const {
    return c;
}

bool UniquePriorityQueue::compareFunction(const WordCount &a, const WordCount &b) {
    return a.count > b.count;
}
