#pragma once

#include <QString>

// that is just a convenience struct
struct WordCount {
    QString word;
    quint64 count{ 0 };
};
