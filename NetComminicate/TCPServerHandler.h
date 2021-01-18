#pragma once
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDateTime>
class TCPServerHandler : public QObject
{
	Q_OBJECT
public:
	TCPServerHandler(QString listen_ip, int listen_port, QObject* parent = Q_NULLPTR);
	~TCPServerHandler();

	QHostAddress m_listen_ip;
	int m_listen_port;
	//int m_receive_port; 
	QTcpServer* m_server;
	QTcpSocket* m_socket;
	 
	void init();

public slots:
	void sendData(QString info);
	void disconnectTcp();
private slots:
	void receiveData();
	void onNewConnection();
	void onClientDisconnected();
	void onClientConnected();
private: 
	void close();

signals:
	void displayInfo(bool is_alarm, QString info);
};

