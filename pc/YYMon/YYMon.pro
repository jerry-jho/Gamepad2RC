TEMPLATE = app
TARGET = YYMon
INCLUDEPATH += .
HEADERS += YGamePad.hpp YGamePadConfig.hpp
SOURCES += main.cpp
CONFIG += c++11 console
QT += gamepad widgets
RESOURCES += qdarkstyle/dark/style.qrc