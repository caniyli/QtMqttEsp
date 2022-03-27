#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mqtt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_buttonLedOn_clicked();

	void on_buttonLedoff_clicked();

	void on_sliderServo_valueChanged(int value);

private:
	Ui::MainWindow *ui;
	Mqtt *mqtt;
};
#endif // MAINWINDOW_H
