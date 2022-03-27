#ifndef MQTT_H
#define MQTT_H

#include <QObject>
#include <qmqtt.h>
#include <QHostInfo>

class Mqtt : public QObject
{
	Q_OBJECT
public:
	explicit Mqtt(QObject *parent = nullptr);
	~Mqtt();
	void set_mqtt_host(QString host);
	void set_mqtt_port(int port);
	void set_mqtt_topic(QString topic);
	void mqtt_connect();
	void mqtt_disconnect();
	void mqtt_unsubscribe(QString topic);
	void mqtt_publish(QString message);

signals:
	void received(QString message);

public slots:
	void mqtt_connected();
	void mqtt_disconnected();
	void mqtt_subscribed();
	void mqtt_unsubscribed();
	void mqtt_received(QMQTT::Message message);

private:
	void mqtt_subscribe(QString topic);
	QString m_host, m_topic;
	int m_port;
	QMQTT::Client *m_client;

};

#endif // MQTT_H
