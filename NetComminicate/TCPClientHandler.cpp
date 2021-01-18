#include "TCPClientHandler.h"
#define MAX_NET_FRAME_LENGTH 65535
#pragma execution_character_set("utf-8")
TCPClientHandler::TCPClientHandler(QString server_ip, int server_port, QObject* parent)
	: QObject(parent)
	, m_server_ip(QHostAddress(server_ip))
	, m_server_port(server_port)
	, m_socket(nullptr)
{
	//init();
}

TCPClientHandler::~TCPClientHandler()
{
	close();
}

void TCPClientHandler::init()
{
	close(); 
	m_socket = new QTcpSocket();
	m_socket->connectToHost(m_server_ip, m_server_port);
	connect(m_socket, &QTcpSocket::disconnected, this, &TCPClientHandler::onClientDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &TCPClientHandler::receiveData); 
	emit displayInfo(true, "Socket初始化成功");
}

void TCPClientHandler::close()
{
	if (nullptr != this && m_socket) {
		m_socket->disconnectFromHost(); 
		emit displayInfo(true, "Tcp客户端断开连接");
	}
}
void TCPClientHandler::disconnectTcp()
{
	//close();
} 
void TCPClientHandler::onClientDisconnected()
{
	//close();
}

void TCPClientHandler::receiveData()
{ 
	//while (m_socket->canReadLine())
	//{
		QString recvMessage = m_socket->readAll();
		emit displayInfo(true, recvMessage);
	//}
}

void TCPClientHandler::sendData(QString info)
{
	QByteArray arr = (tr("[From ") + m_socket->peerAddress().toString() + ":" + QString::number(m_socket->peerPort()) + "] " + info).toUtf8();
	//QByteArray arr = info.toUtf8();
	//arr.append('\n');
	m_socket->write(arr); 
}