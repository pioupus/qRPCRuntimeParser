#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "channel_codec_wrapper.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoder.h"
#include "rpcruntime_encoded_function_call.h"
#include "rpcruntime_encoder.h"
#include "rpcruntime_function.h"
#include "rpcruntime_protocol_description.h"
#include "rpcruntime_transfer.h"

#include <QDebug>
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

static RPCRunTimeProtocolDescription protocol;
static RPCRuntimeEncoder encoder{protocol};
static RPCRuntimeDecoder decoder{protocol};
static Channel_codec_wrapper channel_codec{decoder};

static void write_all_data(QSerialPort &comport, const char *data, int size) {
	if (size == 0){
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
				write_all_data(comport, channel_codec.encode(RPCRuntimeEncodedFunctionCall::create_hash_request()));
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

void MainWindow::received(const RPCRuntimeTransfer &transfer) {
	assert(transfer.is_complete()); //should only call this function with complete transfers
	auto call = transfer.decode();
	QTreeWidgetItem w;
	w.setText(0, QString::fromStdString(call.get_declaration()->get_function_name()));
	ui->replies->addTopLevelItem(new QTreeWidgetItem(std::move(w)));
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
		auto transfer = channel_codec.pop();
		auto item = new QTreeWidgetItem;
		item->setText(0, transfer.get_declaration()->get_function_declaration().c_str());
		ui->replies->addTopLevelItem(item);
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
