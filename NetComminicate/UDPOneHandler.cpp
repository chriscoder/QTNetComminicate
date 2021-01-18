#include "UDPOneHandler.h"
#define MAX_NET_FRAME_LENGTH 65535
#pragma execution_character_set("utf-8")
UDPOneHandler::UDPOneHandler(QString peer_ip, int peer_port, int receive_port, QObject* parent)
	: QObject(parent)
	, m_peer_ip(peer_ip)
	, m_peer_port(peer_port)
	, m_receive_port(receive_port)
	, m_socket(nullptr)
{
	//init();
}

UDPOneHandler::~UDPOneHandler()
{
	close();
}

void UDPOneHandler::init()
{
	close();
	m_socket = new QUdpSocket();

	//���ؼ����˿ں�Ϊ����ָ��
	if (!m_socket->bind(m_receive_port)) {
		emit displayInfo(false, "Socket�˿ڳ�ʼ��ʧ��");
		return;
	}
	connect(m_socket, &QUdpSocket::readyRead, this, &UDPOneHandler::receiveData); 
	emit displayInfo(true, "Socket��ʼ���ɹ�");
}

void UDPOneHandler::close()
{
	if (m_socket) {
		if (m_socket->isOpen())
			m_socket->close();
		delete m_socket;
		m_socket = nullptr;
		m_socket->deleteLater();
		emit displayInfo(false, "�ͷ�����");
	}
}

void UDPOneHandler::receiveData()
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
		std::string ip = peer_addr.toString().toStdString();
		if (0 == datagram.size()) continue;
		iLength = datagram.size();
		  
		QString recvMessage = tr("[From ") + peer_addr.toString() +":" + QString::number(peer_port) + "] " + datagram.data();
		emit displayInfo(true, recvMessage);
	}
}

void UDPOneHandler::sendData(QString info, int type)
{
	QByteArray arr = info.toUtf8();
	int size = 0;
	switch (type)
	{
		case 2:
		{//����
			size = m_socket->writeDatagram(arr, m_peer_ip, m_peer_port);
			break;
		}
		case 4:
		{//�㲥
			size = m_socket->writeDatagram(arr, QHostAddress::Broadcast, m_peer_port);
			break;
		}
	}
	 
	if (size != arr.size()) {
		emit displayInfo(false, "��ͻ��˷�������ʧ��");
	}
}