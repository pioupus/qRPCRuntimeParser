#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_function.h"

#include <QDebug>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtSerialPort/QSerialPortInfo>

#include <cassert>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
    ui->setupUi(this);
	ui->refresh_port_list->click();
	ui->close_button->hide();
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
				QTimer::singleShot(0, this, SLOT(poll()));
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
	ui->log->insertPlainText(comport.readAll());
	QTimer::singleShot(16, this, SLOT(poll()));
}
