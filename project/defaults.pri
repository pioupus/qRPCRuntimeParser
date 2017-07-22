QT = gui core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SRC_DIR = $$PWD

INCLUDEPATH += $$PWD/src

QMAKE_CXXFLAGS += -Werror
CONFIG += warn
CONFIG += c++14
#CONFIG += c++11
#QMAKE_CXXFLAGS += -std=c++14

INCLUDEPATH += $$PWD/libs/include/
INCLUDEPATH += $$PWD/libs/RPC-ChannelCodec/include
