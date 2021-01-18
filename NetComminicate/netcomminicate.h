#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_netcomminicate.h"
#include "clientform.h"
#include "TCPServerHandler.h"
#include <QButtonGroup>
class NetComminicate : public QMainWindow
{
    Q_OBJECT

public:
    NetComminicate(QWidget *parent = Q_NULLPTR);
    ~NetComminicate();
     
private:
    Ui::ServerPeer ui; 
    QButtonGroup* group_type;
    UDPOneHandler* m_udp_one_handler;
    UDPMutilcastHandler* m_udp_mutilcast_handler; 
    TCPServerHandler* m_tcp_server_handler;
    void init();
    void sendData();

    void groupInit();
    void sendGroupData();
    void tcpInit();
    void disconnectTcp();
    void disconnectMutilUDP();
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
