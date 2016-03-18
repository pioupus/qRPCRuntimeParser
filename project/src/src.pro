include(../defaults.pri)

CONFIG( debug, debug|release ) {
    # debug
     TARGET = qTestProjectTemplateAppd
} else {
    # release
    TARGET = qTestProjectTemplateApp

}

DEFINES += CHANNEL_CODEC_RUNS_IN_CPP_CLASS_ENVIRONMENT

QT += xml
TEMPLATE = lib

DEFINES += EXPORT_LIBRARY

SOURCES += main.cpp
SOURCES += rpcruntime_decoder.cpp
SOURCES += rpcruntime_function.cpp
SOURCES += rpcruntime_protocol_description.cpp
SOURCES += mainwindow.cpp




HEADERS += mainwindow.h
HEADERS += rpcruntime_decoder.h
HEADERS += rpcruntime_function.h
HEADERS += rpcruntime_protocol_description.h

SOURCES += ../libs/rpc_channel_codec/src/channel_codec/channel_codec.c
SOURCES += ../libs/rpc_channel_codec/src/channel_codec/crc16.c

HEADERS += ../libs/rpc_channel_codec/include/channel_codec/channel_codec.h

HEADERS += rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h
HEADERS += rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h

INCLUDEPATH += ../libs/rpc_channel_codec/include
INCLUDEPATH += ../libs/rpc_channel_codec/include/errorlogger_dummy


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
