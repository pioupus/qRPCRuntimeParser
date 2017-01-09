include(../defaults.pri)

CONFIG( debug, debug|release ) {
    # debug
     TARGET = qTestProjectTemplateAppd
} else {
    # release
    TARGET = qTestProjectTemplateApp

}


QPROTOCOL_INTERPRETER_PATH = ../..
include($$QPROTOCOL_INTERPRETER_PATH/qProtocollInterpreter_static.pri)

TEMPLATE = lib

DEFINES += EXPORT_LIBRARY
