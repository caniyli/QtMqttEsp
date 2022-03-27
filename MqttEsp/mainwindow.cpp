#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	mqtt = new Mqtt();
	mqtt->set_mqtt_host("192.168.1.30");
	mqtt->set_mqtt_port(1883);
	mqtt->set_mqtt_topic("esp/test");
	mqtt->mqtt_connect();
	connect(mqtt, &Mqtt::received, this, [=](QString message){
										if (message == "ledOk!")
										{
											ui->labelLedOk->setText("Led Yakıldı");
										} else if(message == "ledOff!")
										{
											ui->labelLedOk->setText("Led Söndürüldü");
										} else if(message == "servoOk!")
										{
											ui->labelServoOk->setText("Servo Hareket Etti");
										}
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_buttonLedOn_clicked()
{
	mqtt->mqtt_publish("led:on");
	ui->labelLedOk->setText("Esp'den Cevap Bekleniyor");
}


void MainWindow::on_buttonLedoff_clicked()
{
	mqtt->mqtt_publish("led:off");
	ui->labelLedOk->setText("Esp'den Cevap Bekleniyor");
}


void MainWindow::on_sliderServo_valueChanged(int value)
{
	ui->labelServo->setText(QString::number((value * 182) / 100));
	mqtt->mqtt_publish("servo:" + QString::number((value * 182) / 100));
	ui->labelServoOk->setText("Esp'den Cevap Bekleniyor");
}

