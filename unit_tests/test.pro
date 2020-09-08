######################################################################
# Automatically generated by qmake (3.1) Tue Jul 23 18:20:50 2019
######################################################################

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5")

TEMPLATE = app
TARGET = run_tests
INCLUDEPATH += ../include .
QT += core gui widgets multimedia testlib
CONFIG += c++17 debug testcase

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Input
SOURCES += main.cpp ../src/MainWindow.cpp ../src/RenderArea.cpp
SOURCES += ../src/WindowSetTimeSig.cpp ../src/WindowSetVolume.cpp
SOURCES += ../src/WindowSetBPM.cpp ../src/BeatCircleTimer.cpp
SOURCES += TestQString.cpp

HEADERS += ../include/MainWindow.hpp ../include/RenderArea.hpp
HEADERS += ../include/WindowSetTimeSig.hpp ../include/WindowSetVolume.hpp
HEADERS += ../include/WindowSetBPM.hpp ../include/BeatCircleTimer.hpp
HEADERS += TestQString.hpp
