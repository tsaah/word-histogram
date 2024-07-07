cmake_minimum_required(VERSION 3.16)

project(word_histogram VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(word_histogram_app
    main.cpp
)

qt_add_resources(word_histogram_app "resources"
    PREFIX "/"
    FILES qml/Main.qml
)

qt_add_qml_module(word_histogram_app
    URI my.word_histogram
    VERSION 1.0
    SOURCES
        wordcount.h
        uniquepriorityqueue.h uniquepriorityqueue.cpp
        filewordcounter.h filewordcounter.cpp
        filewordcounterworker.h filewordcounterworker.cpp
        wordfrequencymodel.h wordfrequencymodel.cpp
    QML_FILES
        qml/Button.qml
        qml/ProgressBar.qml
        qml/Histogram.qml
        qml/HistogramBar.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(word_histogram_app PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.word_histogram_app
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(word_histogram_app
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS word_histogram_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
