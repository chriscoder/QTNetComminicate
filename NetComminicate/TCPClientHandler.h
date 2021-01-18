#pragma once
#include <QObject>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QDateTime>
class TCPClientHandler : public QObject
{
	Q_OBJECT
public:
	TCPClientHandler(QString server_ip, int server_port, QObject* parent = Q_NULLPTR);
	~TCPClientHandler();

	QHostAddress m_server_ip;
	int m_server_port; 
	QTcpSocket* m_socket;
	void init();

public slots:
	void sendData(QString info);
	void disconnectTcp();
private slots:
	void receiveData();
	void onClientDisconnected();
private: 
	void close();

signals:
	void displayInfo(bool is_alarm, QString info);
};

