#include "rpcruntime_protcol.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_encoded_function_call.h"
#include <QDirIterator>
#include <assert.h>

RPCRuntimeProtocol::RPCRuntimeProtocol(RPCIODevice &device, std::chrono::steady_clock::duration timeout)
    : decoder{description}
    , encoder{description}
    , channel_codec{decoder}
    , device(&device)
    , device_timeout(timeout) {
    connection = QObject::connect(&device, &RPCIODevice::received, [&cc = channel_codec](const QByteArray &data) {
        //qDebug() << "RPC-Protocol received" << data.size() << "bytes from device";
        cc.add_data(reinterpret_cast<const unsigned char *>(data.data()), data.size());
    });
    assert(connection);
}

std::unique_ptr<RPCRuntimeDecodedFunctionCall> RPCRuntimeProtocol::call_and_wait(const RPCRuntimeEncodedFunctionCall &call) {
    return call_and_wait(call, device_timeout);
}

std::unique_ptr<RPCRuntimeDecodedFunctionCall> RPCRuntimeProtocol::call_and_wait(const RPCRuntimeEncodedFunctionCall &call,
                                                                                 std::chrono::steady_clock::duration duration) {
    for (int try_count = 0; try_count <= retries_per_transmission; try_count++) {
        if (try_count != 0) {
            emit console_message(
                RPCConsoleLevel::debug,
                QString(R"(Request for function "%1" timed out, retry %2)").arg(call.get_description()->get_function_name().c_str()).arg(try_count));
        }

        device->send(channel_codec.encode(call), call.encode());
        // Utility::thread_call(device,nullptr,
        //                      [ device = this->device, data = channel_codec.encode(call), display_data = call.encode() ] { device->send(data, display_data);
        //                      });
        auto start = std::chrono::high_resolution_clock::now();
        auto check_received = [this, &start, &duration, &call]() -> std::unique_ptr<RPCRuntimeDecodedFunctionCall> {
            device->waitReceived(duration - (std::chrono::high_resolution_clock::now() - start), 1);
            if (channel_codec.transfer_complete()) { //found a reply
                auto transfer = channel_codec.pop_completed_transfer();
                auto &raw_data = transfer.get_raw_data();
                emit device->decoded_received(QByteArray(reinterpret_cast<const char *>(raw_data.data()), raw_data.size()));
                auto decoded_call = transfer.decode();
                if (decoded_call.get_id() == call.get_description()->get_reply_id()) { //found correct reply
                    return std::make_unique<RPCRuntimeDecodedFunctionCall>(std::move(decoded_call));
                } else { //found reply to something else, just gonna quietly ignore it
                    emit console_message(
                        RPCConsoleLevel::debug,
                        QString(R"(RPCProtocol::call_and_wait wrong answer. Expected answer to function "%1" but got answer to function "%2" instead.)")
                            .arg(call.get_description()->get_function_name().c_str(), decoded_call.get_declaration()->get_function_name().c_str()));
                }
            }
            return nullptr;
        };
        do {
            auto retval = check_received();
            if (retval) {
                return retval;
            }
        } while (std::chrono::high_resolution_clock::now() - start < duration);
    }
    return nullptr;
}

void RPCRuntimeProtocol::clear() {
    while (channel_codec.transfer_complete()) {
        channel_codec.pop_completed_transfer();
    }
    channel_codec.reset_current_transfer();
}

RPCRuntimeEncodedFunctionCall RPCRuntimeProtocol::encode_function(const std::string &name) const {
    return encoder.encode(name);
}

const channel_codec_instance_t *RPCRuntimeProtocol::debug_get_channel_codec_instance() const {
    return channel_codec.debug_get_instance();
}

bool RPCRuntimeProtocol::load_xml_file(QString search_dir) {
    // const CommunicationDevice::Duration TIMEOUT = std::chrono::milliseconds{100};
    int retries_per_transmission_backup = retries_per_transmission;
    retries_per_transmission = 0;
    RPCRuntimeEncodedFunctionCall get_hash_function = encoder.encode(0);
    RPCRuntimeEncodedParam &param_hash = get_hash_function.get_parameter(0);
    RPCRuntimeEncodedParam &param_hash_index = get_hash_function.get_parameter(1);
    for (std::size_t i = 0; i < 16; i++) {
        param_hash[i].set_value(0); //For now a runtime rpc codec should send a NULL hash, since it knows to speak many hashes.
    }
    param_hash_index[0].set_value(0);
    assert(get_hash_function.are_all_values_set());

    auto result = call_and_wait(get_hash_function, device_timeout);
    retries_per_transmission = retries_per_transmission_backup;
    if (result) {
        const auto &hash = QByteArray::fromStdString(result->get_parameter_by_name("hash_inout")->as_full_string()).toHex();
        device->message(QObject::tr("Received Hash: ").toUtf8() + hash);
        QString folder = search_dir;
        QString filename = hash + ".xml";
        QDirIterator directory_iterator(folder, QStringList{} << filename, QDir::Files, QDirIterator::Subdirectories);
        if (directory_iterator.hasNext() == false) {
            device->message(
                QObject::tr(
                    R"(Failed finding RPC description file "%1" in folder "%2" or any of its subfolders. Make sure it exists or change the search path in the settings menu.)")
                    .arg(folder, filename)
                    .toUtf8());
            return false;
        }
        auto filepath = directory_iterator.next();
        std::ifstream xmlfile(filepath.toStdString());
        if (description.openProtocolDescription(xmlfile) == false) {
            device->message(QObject::tr(R"(Failed opening RPC description file "%1".)").arg(filename).toUtf8());
            return false;
        }
    }

    return result != nullptr;
}

RPCIODevice::RPCIODevice() {}

void RPCIODevice::send(std::vector<unsigned char> data, std::vector<unsigned char> pre_encodec_data) {
    (void)data;
    (void)pre_encodec_data;
}

bool RPCIODevice::waitReceived(std::chrono::_V2::steady_clock::duration timeout, int bytes, bool isPolling) {
    (void)timeout;
    (void)bytes;
    (void)isPolling;
    return false;
}
