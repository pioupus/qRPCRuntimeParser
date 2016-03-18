include(../defaults.pri)

CONFIG( debug, debug|release ) {
    # debug
     TARGET = qTestProjectTemplateAppd
} else {
    # release
    TARGET = qTestProjectTemplateApp

}

QT += xml
TEMPLATE = lib

DEFINES += EXPORT_LIBRARY

SOURCES += main.cpp
SOURCES += rpcruntimedecoderesult.cpp
SOURCES += rpcruntimefunction.cpp
SOURCES += rpcruntimeinterpreter.cpp
SOURCES += mainwindow.cpp




HEADERS += mainwindow.h
HEADERS += rpcruntimedecoderesult.h
HEADERS += rpcruntimefunction.h
HEADERS += rpcruntimeinterpreter.h

SOURCES += ../libs/rpc_channel_codec/src/channel_codec/channel_codec.c
SOURCES += ../libs/rpc_channel_codec/src/channel_codec/crc16.c

INCLUDEPATH += ../libs/rpc_channel_codec/include
INCLUDEPATH += ../libs/rpc_channel_codec/include/errorlogger_dummy
INCLUDEPATH += ../libs/rpc_channel_codec/tests/include


FORMS    += mainwindow.ui

#INCLUDEPATH += $$(PYTHON_PATH)/include

#exists( $$(PYTHON_PATH)/include/python.h ) {
#    #message(found python windows)
#    INCLUDEPATH += $$(PYTHON_PATH)/include
#    PYTHON_FOUND = 1
#}

#exists( $$(PYTHON_PATH)/Python.h ) {
    #message(found python linux)crystalTestFrameworkApp
#    INCLUDEPATH += $$(PYTHON_PATH)
#    PYTHON_FOUND = 1
#}

#!equals( PYTHON_FOUND , 1){
#    error (Python directory needs to be configured in environment variable PYTHON_PATH. eg. C:/Python27 )
#}
