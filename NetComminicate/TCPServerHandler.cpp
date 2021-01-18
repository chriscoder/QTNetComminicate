#include "TCPServerHandler.h"
#define MAX_NET_FRAME_LENGTH 65535
#pragma execution_character_set("utf-8")
TCPServerHandler::TCPServerHandler(QString listen_ip, int listen_port, QObject* parent)
	: QObject(parent)
	, m_listen_ip(listen_ip)
	, m_listen_port(listen_port)
	, m_server(nullptr)
	, m_socket(nullptr)
{
	//init();
}

TCPServerHandler::~TCPServerHandler()
{
	disconnectTcp();
}

void TCPServerHandler::init()
{
	close();
	if (m_server != nullptr && m_server->isListening())
	{
		m_server->close();
	}
	m_server = new QTcpServer();
	m_server->listen(m_listen_ip, m_listen_port);
	connect(m_server, &QTcpServer::newConnection, this, &TCPServerHandler::onNewConnection); 
	emit displayInfo(true, "Socket初始化成功");
}

void TCPServerHandler::close()
{
	if (m_socket) {
		m_socket->close();
		m_socket->deleteLater();
		m_socket = nullptr;
		emit displayInfo(true, "Tcp服务器断开连接");
	}
}
void TCPServerHandler::disconnectTcp()
{
	close();
	if (m_server != nullptr && m_server->isListening())
	{
		m_server->close();
	}
	emit displayInfo(true, "Tcp服务器停止监听");
}
void TCPServerHandler::onNewConnection()
{
	m_socket = m_server->nextPendingConnection(); 
	connect(m_socket, &QTcpSocket::disconnected, this, &TCPServerHandler::onClientDisconnected);
	connect(m_socket, &QTcpSocket::connected, this, &TCPServerHandler::onClientConnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &TCPServerHandler::receiveData);

	emit displayInfo(true, "与客户端[" + m_socket->peerAddress().toString() + ":" + QString::number(m_socket->peerPort()) + "] Tcp连接成功！"); 
}

void TCPServerHandler::onClientConnected()
{
	//close();
}

void TCPServerHandler::onClientDisconnected()
{
	close(); 
}

void TCPServerHandler::receiveData()
{ 
	//while (m_socket->canReadLine())
	//{
		QString recvMessage = m_socket->readAll();
		emit displayInfo(true, recvMessage);
	//}
}

void TCPServerHandler::sendData(QString info)
{
	if (m_socket == nullptr)
	{
		emit displayInfo(true, "Tcp服务器断开连接");
		return;
	}
	QByteArray arr = (tr("[From ") + m_socket->peerAddress().toString() + ":" + QString::number(m_socket->peerPort()) + "] " + info).toUtf8();
	//arr.append('\n');
	m_socket->write(arr); 
}