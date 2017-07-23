#ifndef RPCRUNTIMEPROTCOL_H
#define RPCRUNTIMEPROTCOL_H

#include "rpcruntime_protocol_description.h"
#include "rpcruntime_decoder.h"
#include "rpcruntime_encoder.h"
#include "rpcruntime_decoded_function_call.h"
#include "channel_codec_wrapper.h"
#include <QObject>
#include <chrono>
#include <QByteArray>
#include <memory>

enum class RPCConsoleLevel {note,debug,warning,error} ;

class RPCIODevice: public QObject{
    Q_OBJECT
public:
    RPCIODevice();
    virtual void send(std::vector<unsigned char> data, std::vector<unsigned char> pre_encodec_data);

    virtual bool waitReceived(std::chrono::steady_clock::duration timeout = std::chrono::seconds(1), int bytes = 1);

signals:
    void decoded_received(const QByteArray &);
    void message(const QByteArray &);

    void received(const QByteArray &);

};

class RPCRuntimeProtocol : public QObject
{
    Q_OBJECT
public:
    RPCRuntimeProtocol(RPCIODevice &device, std::chrono::steady_clock::duration timeout);


    std::unique_ptr<RPCRuntimeDecodedFunctionCall> call_and_wait(const RPCRuntimeEncodedFunctionCall &call);
    std::unique_ptr<RPCRuntimeDecodedFunctionCall> call_and_wait(const RPCRuntimeEncodedFunctionCall &call, std::chrono::steady_clock::duration duration);
    void clear();
    RPCRuntimeEncodedFunctionCall encode_function(const std::string &name) const;
    const channel_codec_instance_t *debug_get_channel_codec_instance() const;
    bool load_xml_file(QString search_dir);
    int retries_per_transmission{2};

    RPCRunTimeProtocolDescription description;
signals:
    void console_message(RPCConsoleLevel level,QString message);
private:
    RPCRuntimeDecoder decoder;
    RPCRuntimeEncoder encoder;
    Channel_codec_wrapper channel_codec;
    QMetaObject::Connection connection;
    std::unique_ptr<RPCRuntimeDecodedFunctionCall> descriptor_answer;
    RPCIODevice *device = nullptr;

    std::chrono::steady_clock::duration device_timeout;
};

#endif // RPCRUNTIMEPROTCOL_H
