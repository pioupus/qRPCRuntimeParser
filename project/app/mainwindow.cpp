#include "mainwindow.h"
#include "ui_mainwindow.h"



#include "channel_codec_wrapper.h"
//#include "global.h"
#include "rpc_ui.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoder.h"
#include "rpcruntime_encoded_function_call.h"
#include "rpcruntime_encoder.h"
#include "rpcruntime_function.h"
#include "rpcruntime_protocol_description.h"
#include "rpcruntime_transfer.h"

#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtSerialPort/QSerialPortInfo>

#include <cassert>
#include <fstream>

#if 0
GLOBAL(RPCRunTimeProtocolDescription, protocol)
GLOBAL(RPCRuntimeEncoder, encoder, global::protocol)
GLOBAL(RPCRuntimeDecoder, decoder, global::protocol)
GLOBAL(Channel_codec_wrapper, channel_codec, global::decoder)
#else
RPCRunTimeProtocolDescription protocol;
RPCRuntimeEncoder  encoder(protocol);//, global::protocol
RPCRuntimeDecoder  decoder(protocol);//, global::protocol
Channel_codec_wrapper  channel_codec(decoder);//, global::decoder
#endif
static void write_all_data(QSerialPort &comport, const char *data, int size) {
	if (size == 0) {
		return;
	}
	qDebug() << std::string(data, data + size).c_str();
	for (int i = 0; i < size;) {
		i += comport.write(data + i, size - i);
	}
}

static void write_all_data(QSerialPort &comport, const std::vector<unsigned char> &data) {
	write_all_data(comport, reinterpret_cast<const char *>(data.data()), data.size());
}

static void load_protocol_description(QWidget *parent, const std::string &hex_hash) {
	const auto xml_path = QSettings{}.value("xml path").toString().toStdString() + hex_hash + ".xml";
	std::ifstream xml_file(xml_path);
	if (!xml_file) {
		QMessageBox::warning(parent, "Invalid Hash", ("The file " + xml_path + " was not found.").c_str());
		return;
	}
    if (protocol.openProtocolDescription(xml_file) == false) {
		QMessageBox::warning(parent, "Parsing error", ("The file " + xml_path + " is not a valid protocol description.").c_str());
	}
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
    ui->setupUi(this);
	ui->refresh_port_list->click();
	ui->close_button->hide();
	QCoreApplication::setOrganizationName("CPG");
	QCoreApplication::setApplicationName("RPC Runtime Decoder");
	(void)load_protocol_description;
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::refresh_comports() {
	const auto &selected_port = ui->port_list->currentText();
	ui->port_list->clear();
	for (auto &port : QSerialPortInfo::availablePorts()) {
		ui->port_list->addItem(port.portName());
	}
	ui->port_list->setCurrentText(selected_port);
}

void MainWindow::open_comport() {
	const auto &selected_port = ui->port_list->currentText();
	for (auto &port : QSerialPortInfo::availablePorts()) {
		if (selected_port == port.portName()) {
			comport.setPort(port);
			if (comport.open(QIODevice::ReadWrite)) {
				comport.setBaudRate(QSerialPort::BaudRate::Baud115200);
				ui->open_comport->hide();
				ui->close_button->show();
                write_all_data(comport, channel_codec.encode(encoder.encode(0)));
				QTimer::singleShot(0, this, &MainWindow::poll);
			}
			return;
		}
	}
	//didn't find that comport anymore
	ui->refresh_port_list->click();
}

void MainWindow::close_comport() {
	comport.close();
	ui->open_comport->show();
	ui->close_button->hide();
	ui->refresh_port_list->click();
}

void MainWindow::send_request() {
	auto item = ui->requests->currentItem();
	if (item == nullptr) {
		return;
	}
	while (item->parent() != nullptr){
		item = item->parent();
	}
	const auto &function_name = item->text(0).toStdString();
    RPCRuntimeEncodedFunctionCall function_call = encoder.encode(function_name);
	if (function_call.are_all_values_set()) {
        const auto &data = channel_codec.encode(function_call);
		write_all_data(comport, data);
	} else {
		QMessageBox::information(this, "RPC Error", "Required parameters for the request " +
														QString::fromStdString(function_call.get_description()->get_function_name()) + " were not set");
	}
}

void MainWindow::received(const RPCRuntimeTransfer &transfer) {
	assert(transfer.is_complete()); //should only call this function with complete transfers
	auto call = transfer.decode();
	QTreeWidgetItem w;
	w.setText(0, QString::fromStdString(call.get_declaration()->get_function_name()));
	ui->replies->addTopLevelItem(new QTreeWidgetItem(std::move(w)));
}

static char to_hex_char(int character) {
	assert(character >= 0);
	assert(character < 16);
	if (character < 10) {
		return character + '0';
	}
	return character - 10 + 'A';
}

void MainWindow::poll() {
	if (comport.isOpen() == false) {
		return;
	}
	const auto old_cursor = ui->log->textCursor();
	auto new_cursor = old_cursor;
	new_cursor.movePosition(QTextCursor::End);
	ui->log->setTextCursor(new_cursor);
	const auto &data = comport.readAll();
    channel_codec.add_data(reinterpret_cast<const unsigned char *>(data.data()), data.size());
    if (channel_codec.transfer_complete()) {
        RPCRuntimeDecodedFunctionCall function_call = channel_codec.pop();
		ui->replies->addTopLevelItem(getTreeWidgetReport(function_call).release());
		if (function_call.get_declaration()->get_reply_id() == 1) { //got a hash reply
			const auto &hash_array = function_call.get_decoded_parameters()[0].as_array();
			assert(hash_array.size() == 16);
			std::string hash;
			for (auto &byte : hash_array) {
				int byte_value = byte.as_integer();
				hash += to_hex_char(byte_value >> 4);
				hash += to_hex_char(byte_value & 0x0F);
			}
			std::ifstream f("../../protocols/" + hash + ".xml");
            if (protocol.openProtocolDescription(f)) { //successfully loaded protocol
				ui->requests->clear();
                for (auto &f : protocol.get_functions()) {
                    ui->requests->addTopLevelItem(getTreeWidgetReport(encoder.encode(f.get_request_id())).release());
				}
				ui->requests->header()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
			} else { //failed loading protocol
				QMessageBox::critical(this, "Protocol Error", "The file " +
																  QFileInfo("../../protocols/" + QString::fromStdString(hash) + ".xml").absoluteFilePath() +
																  " could not be loaded.");
			}
		}
	}

	ui->log->insertPlainText(data);
	if (QApplication::mouseButtons() & Qt::LeftButton) { //the user is actively selecting something, don't mess with it
		ui->log->setTextCursor(old_cursor);
	} else if (old_cursor.selection().isEmpty()) { //user currently has nothing selected, jump to the end
		new_cursor.movePosition(QTextCursor::End);
		ui->log->setTextCursor(new_cursor);
	} else { //user had something selected, restore selection
		ui->log->setTextCursor(old_cursor);
	}
	QTimer::singleShot(16, this, &MainWindow::poll);
}
