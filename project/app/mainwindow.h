#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

class RPCRuntimeTransfer;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

	public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	public slots:
	void refresh_comports();
	void open_comport();
	void close_comport();
	void send_request();
	private slots:
	void received(const RPCRuntimeTransfer &transfer);
	void poll();

	private:
    Ui::MainWindow *ui;
	QSerialPort comport;
};

#endif // MAINWINDOW_H
