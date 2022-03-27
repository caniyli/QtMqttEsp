#include "mqtt.h"

Mqtt::Mqtt(QObject *parent)
	: QObject{parent}
{

}

void Mqtt::set_mqtt_host(QString host)
{
	m_host = host;
}

void Mqtt::set_mqtt_port(int port)
{
	m_port = port;
}

void Mqtt::set_mqtt_topic(QString topic)
{
	m_topic = topic;
}

void Mqtt::mqtt_connect()
{
	m_client = new QMQTT::Client(m_host, m_port, false, false);
	connect(m_client, &QMQTT::Client::connected, this, &Mqtt::mqtt_connected);
	connect(m_client, &QMQTT::Client::disconnected, this, &Mqtt::mqtt_disconnected);
	connect(m_client, &QMQTT::Client::subscribed, this, &Mqtt::mqtt_subscribed);
	connect(m_client, &QMQTT::Client::unsubscribed, this, &Mqtt::mqtt_unsubscribed);
	connect(m_client, &QMQTT::Client::received, this, &Mqtt::mqtt_received);
	m_client->connectToHost();
}

void Mqtt::mqtt_connected()
{
	static QMQTT::Message msg;
	msg.setPayload(QString(QHostInfo::localHostName()+":online").toUtf8());
	msg.setTopic(m_topic + "/send");
	m_client->publish(msg);
	mqtt_subscribe(m_topic + "/receive");
}

void Mqtt::mqtt_disconnected()
{
	qDebug() << "Disconnected";
}

void Mqtt::mqtt_subscribed()
{
	qDebug() << "Subscribed";
}

void Mqtt::mqtt_unsubscribed()
{
	qDebug() << "Unsubscribed";
}


void Mqtt::mqtt_disconnect()
{
	m_client->disconnectFromHost();
}

void Mqtt::mqtt_subscribe(QString topic)
{
	m_client->subscribe(topic);
}

void Mqtt::mqtt_unsubscribe(QString topic)
{
	m_client->unsubscribe(topic);
}

void Mqtt::mqtt_publish(QString message)
{
	static QMQTT::Message msg;
	msg.setPayload(message.toUtf8());
	msg.setTopic(m_topic + "/send");
	m_client->publish(msg);
}

void Mqtt::mqtt_received(QMQTT::Message message)
{
	emit received(message.payload());
}

Mqtt::~Mqtt()
{
	delete m_client;
}


