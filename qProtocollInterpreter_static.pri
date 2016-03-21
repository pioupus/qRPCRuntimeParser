
DEFINES += CHANNEL_CODEC_RUNS_IN_CPP_CLASS_ENVIRONMENT

QT += xml


QPROTOCOL_INTERPRETER_PATH = $$QPROTOCOL_INTERPRETER_PATH/project

SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_decoder.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_function.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_protocol_description.cpp

HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_decoder.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_function.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_protocol_description.h

SOURCES += $$QPROTOCOL_INTERPRETER_PATH/libs/rpc_channel_codec/src/channel_codec/channel_codec.c
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/libs/rpc_channel_codec/src/channel_codec/crc16.c

HEADERS += $$QPROTOCOL_INTERPRETER_PATH/libs/rpc_channel_codec/include/channel_codec/channel_codec.h

HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h

INCLUDEPATH += $$QPROTOCOL_INTERPRETER_PATH/libs/rpc_channel_codec/include
INCLUDEPATH += $$QPROTOCOL_INTERPRETER_PATH/libs/rpc_channel_codec/include/errorlogger_dummy
INCLUDEPATH += $$QPROTOCOL_INTERPRETER_PATH/libs/rpc_channel_codec/tests/include

