
DEFINES += CHANNEL_CODEC_RUNS_IN_CPP_CLASS_ENVIRONMENT

QT += xml


QPROTOCOL_INTERPRETER_PATH = $$QPROTOCOL_INTERPRETER_PATH/project

SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_decoder.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_function.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_protocol_description.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_paramter_description.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_transfer.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpc_watch_point.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_decoded_param.cpp
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/src/channel_codec_impl.cpp

HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_decoder.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_function.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_protocol_description.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_paramter_description.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_transfer.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpc_watch_point.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpcruntime_decoded_param.h

SOURCES += $$QPROTOCOL_INTERPRETER_PATH/libs/RPC-ChannelCodec/src/channel_codec/channel_codec.c
SOURCES += $$QPROTOCOL_INTERPRETER_PATH/libs/RPC-ChannelCodec/src/channel_codec/crc16.c

HEADERS += $$QPROTOCOL_INTERPRETER_PATH/libs/RPC-ChannelCodec/include/channel_codec/channel_codec.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/libs/RPC-ChannelCodec/include/channel_codec/channel_codec_types.h

HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/src/rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h
HEADERS += $$QPROTOCOL_INTERPRETER_PATH/libs/include/channel_codec/channel_codec_config.h

INCLUDEPATH += $$QPROTOCOL_INTERPRETER_PATH/libs/RPC-ChannelCodec/include
INCLUDEPATH += $$QPROTOCOL_INTERPRETER_PATH/libs/include/
INCLUDEPATH += $$QPROTOCOL_INTERPRETER_PATH/libs/RPC-ChannelCodec/include/errorlogger_dummy
INCLUDEPATH += $$QPROTOCOL_INTERPRETER_PATH/libs/RPC-ChannelCodec/tests/include
