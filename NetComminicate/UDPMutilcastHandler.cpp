#include "UDPMutilcastHandler.h"
#define MAX_NET_FRAME_LENGTH 65535
#pragma execution_character_set("utf-8")
UDPMutilcastHandler::UDPMutilcastHandler(QString group_ip, int group_port, QObject* parent)
	: QObject(parent)
	, m_group_ip(group_ip)
	, m_group_port(group_port)
	, m_socket(nullptr)
{
	//init();
}

UDPMutilcastHandler::~UDPMutilcastHandler()
{
	close();
}
void UDPMutilcastHandler::disconnectUDP()
{
	close();
}
void UDPMutilcastHandler::init()
{
	close();
	m_socket = new QUdpSocket();
	m_socket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
	//本地监听端口号为随意指定
	if (!m_socket->bind(QHostAddress::AnyIPv4, m_group_port, QUdpSocket::ShareAddress)) {
		emit displayInfo(false, "Socket端口初始化失败");
		return;
	}
	m_socket->joinMulticastGroup(m_group_ip); //加入组播
	connect(m_socket, &QUdpSocket::readyRead, this, &UDPMutilcastHandler::receiveData);
	emit displayInfo(true, "加入组播");
}

void UDPMutilcastHandler::close()
{
	if (m_socket) {
		m_socket->leaveMulticastGroup(m_group_ip);
		if (m_socket->isOpen())
			m_socket->close();
		delete m_socket;
		m_socket = nullptr;
		m_socket->deleteLater();
		emit displayInfo(true, "移除组播");
	}
}

void UDPMutilcastHandler::receiveData()
{
	unsigned char data_buff[MAX_NET_FRAME_LENGTH];
	int lastSize = 0;
	while (m_socket->hasPendingDatagrams())
	{
		int iLength;
		QHostAddress peer_addr;
		quint16 peer_port;
		QByteArray datagram;
		datagram.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(datagram.data(), datagram.size(), &peer_addr, &peer_port);
		if (0 == datagram.size()) continue;
		iLength = datagram.size();

		QString recvMessage = tr("[From ") + peer_addr.toString() + ":" + QString::number(peer_port) + "] " + datagram.data();
		emit displayInfo(true, recvMessage);
	}
}

void UDPMutilcastHandler::sendData(QString info)
{
	QByteArray arr = info.toUtf8();
	int size = m_socket->writeDatagram(arr, m_group_ip, m_group_port); 
	if (size != arr.size()) {
		emit displayInfo(false, "向客户端发送数据失败");
	}
}