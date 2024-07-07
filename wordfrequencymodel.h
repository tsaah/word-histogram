#pragma once

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

#include "filewordcounter.h"

// Model for top 15 words data
class WordFrequencyModel: public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(WordCountModel) // in Qt6 this macro will register this class as a QML class in a module defined in a cmake
    Q_PROPERTY(FileWordCounter* fileWordCounter READ fileWordCounter WRITE setFileWordCounter NOTIFY fileWordCounterChanged FINAL REQUIRED) // this property will hold actual source of our data
    Q_PROPERTY(quint64 mostUsedWordCount READ mostUsedWordCount WRITE setMostUsedWordCount NOTIFY mostUsedWordCountChanged FINAL) // this property will hold a biggest word count for later histogram normalisation
public:
    explicit WordFrequencyModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    FileWordCounter* fileWordCounter() const;
    void setFileWordCounter(FileWordCounter* newFileWordCounter);
    quint64 mostUsedWordCount() const;

public slots:
    void clear(); // this will reset model and mostUsedWordCount property to initial state

private slots:
    // this update expects a sorted vector of top used words
    void updateData(std::vector<WordCount> snapshot);

signals:
    void fileWordCounterChanged();
    void mostUsedWordCountChanged();

private:
    void setMostUsedWordCount(quint64 newMostUsedWordCount); // this setter is used privatly during model updates

    static const int WORD_ROLE;
    static const int COUNT_ROLE;
    static const QHash<int, QByteArray> ROLES;

    std::vector<WordCount> dataStorage_; // here we are storing our data
    FileWordCounter* fileWordCounter_ = nullptr;
    quint64 mostUsedWordCount_{ 0 };
};

