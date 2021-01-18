#pragma once
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QDateTime>
class UDPOneHandler : public QObject
{
	Q_OBJECT
public:
	UDPOneHandler(QString server_ip, int server_port, int receive_port, QObject* parent = Q_NULLPTR);
	~UDPOneHandler();

	QHostAddress m_peer_ip;
	int m_peer_port;
	int m_receive_port; 
	QUdpSocket* m_socket;
	void init();

public slots:
	void sendData(QString info, int type);

private slots:
	void receiveData();
	 
private: 
	void close();

signals:
	void displayInfo(bool is_alarm, QString info);
};

