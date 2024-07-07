# Word Histogram
This is a Qt6 QML c++20 app that reads through a file and incrementally shows histogram of top 15 words occurring in it.

![Word Histogram screenshot](/screenshot.png "Word Histogram screenshot.")

## Requirements
- Qt6.5+
- CMake 3.16+

## How it works
### Basic idea
- After start You will be presented with a screen where You can drop or open a text file.
- After opening a text file histogram view will become visible which will present top 15 frequent words appearing in a file incrementally.
- There is a progress bar that will visually display a progress percentage.
- And a couple of buttons. one for Pause/Resume and the other to Stop the process and return to file open screen.
- The histogram view is a Repeater object that uses a model in order to visualize data
- It requires only basic QtQuick and Layout components

### QML Ui
- Main
	- Describes main window, couple of screens and general behavior of application
- ProgressBar
	- Couple of rectangles and a Text to represent a well-known widget.
- Button
	- Even simpler 'Rectangle - Text - Mouse Area' conglomeration to represent a Button
- Histogram
	- A Rectangle with a Layout having a Repeater to create histogram bars based on a model
- HistogramBar
	- A Rectangle with a Text

### C++ side
- WordFrequencyModel class
	- WordFrequencyModel is derived from QAbstaractListModel.
	- It is exposed as a QML type.
	- It is instantiated inside Main.qml.
- FileWordCounter class
	- It's main goal is to operate it's threaded worker and provide way of interacting with it.
	- It forwards a signal with a data updates for the model.
	- It is also exposed as a QML type.
	- It is instantiated inside Main.qml.
- FileWordCounterWorker class
	- It does the heavy lifting as it's main objective is to read file and incrementally track top used words.
	- It is created for every new file.
	- It is running in a separate thread.
	- It provides latest data snapshots roughly every 16 milliseconds.
	- It uses queued connections and c++20 atomics features in order to be interacted with.
- UniquePriorityQueue class
	- It is a customized std::priority_queue that is able to incrementally track top frequently used words.
