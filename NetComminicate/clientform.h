#pragma once
#include <QtNetwork/QNetworkInterface.h>
#include <QtNetwork/QHostAddress.h>
#include <QWidget>
#include "ui_ClientForm.h"
#include "UDPOneHandler.h"
#include "UDPMutilcastHandler.h"
#include "TCPClientHandler.h"
class clientform : public QWidget
{
    Q_OBJECT

public:
    clientform(QWidget *parent = Q_NULLPTR);
    ~clientform();
private:
    Ui::ClientForm ui;
    QButtonGroup* group_type;
    UDPOneHandler * m_udp_one_handler;
    UDPMutilcastHandler* m_udp_mutilcast_handler;
    TCPClientHandler* m_tcp_client_handler;
    void init();
    void sendData();

    void groupInit();
    void sendGroupData();

    void disconnectMutilUDP();

    void tcpInit();
    void tcpDisconnect();
    void disconnectUdp();

    void radioBtnTCPSlot();
    void radioBtnUDPOneSlot();
    void radioBtnUDPMutilSlot();
    void radioBtnUDPBroadCastSlot();

    void choose(int type);

private slots:
    void displayInfo(bool, QString); 
    QString getLocalIP();
signals:
    void sendMsg(QString info, int type);
    void sendGroupMsg(QString info);
    void sendTcpMsg(QString info);
};
