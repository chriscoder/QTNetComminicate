#pragma once
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QDateTime>

class UDPMutilcastHandler : public QObject
{
	Q_OBJECT
public:
	UDPMutilcastHandler(QString group_ip, int group_port, QObject* parent = Q_NULLPTR);
	~UDPMutilcastHandler();

	QHostAddress m_group_ip;
	int m_group_port; 
	QUdpSocket* m_socket;
	void init();

private:
	void close();

public slots:
	void sendData(QString info);
	void disconnectUDP();
private slots:
	void receiveData();

signals:
	void displayInfo(bool is_alarm, QString info);
};

