#include "netcomminicate.h"
#pragma execution_character_set("utf-8")
NetComminicate::NetComminicate(QWidget *parent)
    : QMainWindow(parent)
    , m_udp_one_handler(nullptr)
    , m_udp_mutilcast_handler(nullptr)
{
    ui.setupUi(this);
    clientform* m_clientform = new clientform();
    m_clientform->show();
    QString localIP = getLocalIP();
    this->setWindowTitle(this->windowTitle() + "-- IP:" + localIP);
    connect(ui.pb_init, &QPushButton::clicked, this, &NetComminicate::init);
    connect(ui.pb_send, &QPushButton::clicked, this, &NetComminicate::sendData);  
    connect(ui.pb_group_init, &QPushButton::clicked, this, &NetComminicate::groupInit); 
    connect(ui.pb_tcp_listen, &QPushButton::clicked, this, &NetComminicate::tcpInit);

    connect(ui.rb_tcp, &QRadioButton::toggled, this, &NetComminicate::radioBtnTCPSlot);
    connect(ui.rb_udp_one, &QRadioButton::toggled, this, &NetComminicate::radioBtnUDPOneSlot);
    connect(ui.rb_udp_group, &QRadioButton::toggled, this, &NetComminicate::radioBtnUDPMutilSlot);
    connect(ui.rb_udp_broadcast, &QRadioButton::toggled, this, &NetComminicate::radioBtnUDPBroadCastSlot);

    group_type = new QButtonGroup();
    group_type->addButton(ui.rb_tcp, 1);
    group_type->addButton(ui.rb_udp_one, 2);
    group_type->addButton(ui.rb_udp_group, 3);
    group_type->addButton(ui.rb_udp_broadcast, 4);

    choose(2);
}

 
void NetComminicate::radioBtnTCPSlot()
{
    if (ui.rb_tcp->isChecked())
    {
        choose(1);
    }
}
void NetComminicate::radioBtnUDPOneSlot()
{
    if (ui.rb_udp_one->isChecked())
    {
        choose(2);
    }
}
void NetComminicate::radioBtnUDPMutilSlot()
{
    if (ui.rb_udp_group->isChecked())
    {
        choose(3);
    }
}
void NetComminicate::radioBtnUDPBroadCastSlot()
{
    if (ui.rb_udp_broadcast->isChecked())
    {
        choose(4);
    }
}

void NetComminicate::choose(int type)
{
    ui.le_client_ip->setEnabled(false);
    ui.le_client_port->setEnabled(false);
    ui.le_listen_port->setEnabled(false);
    ui.pb_init->setEnabled(false);
    ui.pb_remove->setEnabled(false);

    ui.le_group_ip->setEnabled(false);
    ui.le_group_port->setEnabled(false);
    ui.pb_group_init->setEnabled(false);
    ui.pb_group_remove->setEnabled(false);

    ui.le_tcp_listen_ip->setEnabled(false);
    ui.le_tcp_listen_port->setEnabled(false);
    ui.pb_tcp_listen->setEnabled(false);
    ui.pb_tcp_stop->setEnabled(false);

    if (type == 2 || type == 4)
    {
        ui.le_client_ip->setEnabled(true);
        ui.le_client_port->setEnabled(true);
        ui.le_listen_port->setEnabled(true);
        ui.pb_init->setEnabled(true);
        ui.pb_remove->setEnabled(true);
    }

    if (type == 1)
    {
        ui.le_tcp_listen_ip->setEnabled(true);
        ui.le_tcp_listen_port->setEnabled(true);
        ui.pb_tcp_listen->setEnabled(true);
        ui.pb_tcp_stop->setEnabled(true);
    }

    if (type == 3)
    {
        ui.le_group_ip->setEnabled(true);
        ui.le_group_port->setEnabled(true);
        ui.pb_group_init->setEnabled(true);
        ui.pb_group_remove->setEnabled(true);
    }
}

NetComminicate::~NetComminicate()
{
    if (m_udp_one_handler)
    {
        delete m_udp_one_handler;
        m_udp_one_handler = nullptr;
    }
}

void NetComminicate::init()
{
    QString _ip = ui.le_client_ip->text();
    int _port = ui.le_client_port->text().toInt();
    int receive_port = ui.le_listen_port->text().toInt();
    m_udp_one_handler = new UDPOneHandler(_ip, _port, receive_port);
    connect(m_udp_one_handler, &UDPOneHandler::displayInfo, this, &NetComminicate::displayInfo);
    connect(this, &NetComminicate::sendMsg, m_udp_one_handler, &UDPOneHandler::sendData);
    connect(ui.pb_remove, &QPushButton::clicked, this, &NetComminicate::disconnectUdp);
    m_udp_one_handler->init();
}
void NetComminicate::disconnectUdp()
{
    if (m_udp_one_handler)
    {
        delete m_udp_one_handler;
        m_udp_one_handler = nullptr;
    }
}
void NetComminicate::sendData()
{
    QString info = ui.te_send_info->toPlainText(); 
    int type = group_type->checkedId();
    switch (type)
    {
        case 1:
        { //TCP
            sendTcpMsg(info);
            break;
        }
        case 2:
        { //UDP单播
            emit sendMsg(info, type);
            break;
        }
        case 3:
        { //UDP组播
            emit sendGroupMsg(info);
            break;
        }
        case 4:
        { //UDP广播
            emit sendMsg(info, type);
            break;
        }
    }
    
    displayInfo(false, "自己:" + info);
}

void NetComminicate::tcpInit()
{
    QString _ip = ui.le_tcp_listen_ip->text();
    int _port = ui.le_tcp_listen_port->text().toInt();
    m_tcp_server_handler = new TCPServerHandler(_ip, _port);
    connect(m_tcp_server_handler, &TCPServerHandler::displayInfo, this, &NetComminicate::displayInfo);
    connect(this, &NetComminicate::sendTcpMsg, m_tcp_server_handler, &TCPServerHandler::sendData);
    connect(ui.pb_tcp_stop, &QPushButton::clicked, this, &NetComminicate::disconnectTcp);
    m_tcp_server_handler->init();
}

void NetComminicate::disconnectTcp()
{
    if (m_tcp_server_handler)
    {
        delete m_tcp_server_handler;
        m_tcp_server_handler = nullptr;
    }
}

void NetComminicate::groupInit()
{
    QString _ip = ui.le_group_ip->text();
    int _port = ui.le_group_port->text().toInt(); 
    m_udp_mutilcast_handler = new UDPMutilcastHandler(_ip, _port);
    connect(m_udp_mutilcast_handler, &UDPMutilcastHandler::displayInfo, this, &NetComminicate::displayInfo);
    connect(this, &NetComminicate::sendGroupMsg, m_udp_mutilcast_handler, &UDPMutilcastHandler::sendData); 
    connect(ui.pb_group_remove, &QPushButton::clicked, this, &NetComminicate::disconnectMutilUDP);
    m_udp_mutilcast_handler->init();
}

void NetComminicate::disconnectMutilUDP()
{
    if (m_udp_mutilcast_handler)
    {
        delete m_udp_mutilcast_handler;
        m_udp_mutilcast_handler = nullptr;
    }
}

void NetComminicate::sendGroupData()
{
    QString info = ui.te_send_info->toPlainText();
    emit sendGroupMsg(info);
    displayInfo(false, "自己:" + info);
}

void NetComminicate::displayInfo(bool is_alarm, QString info)
{
    QString temp_str = QString("%1\n").arg(info);

    ui.textBrowser->insertPlainText(temp_str);
    ui.textBrowser->moveCursor(QTextCursor::Start);//始终定为在第一行
    QTextCursor cursor = ui.textBrowser->textCursor();
    cursor.select(QTextCursor::LineUnderCursor);   //选中要着色的内容
    QTextCharFormat fmt;
    fmt.setForeground(is_alarm ? QColor(Qt::red) : QColor(Qt::black));
    cursor.mergeCharFormat(fmt);    //设置文本格式
    cursor.clearSelection(); //撤销选中
    //cursor.movePosition(QTextCursor::EndOfLine);  //cursor和anchor均移至末尾  
}
 
QString NetComminicate::getLocalIP()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            strIpAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 如果没有找到，则以本地IP地址为IP
    if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return strIpAddress;
}