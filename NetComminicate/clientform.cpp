#include "clientform.h"
#pragma execution_character_set("utf-8")
clientform::clientform(QWidget *parent)
    : QWidget(parent)
    , m_udp_one_handler(nullptr)
{
    ui.setupUi(this);
    QString localIP = getLocalIP();
    this->setWindowTitle(this-> windowTitle() + "-- IP:" + localIP);
    connect(ui.pb_init, &QPushButton::clicked, this, &clientform::init);
    connect(ui.pb_send, &QPushButton::clicked, this, &clientform::sendData); 
    connect(ui.pb_group_init, &QPushButton::clicked, this, &clientform::groupInit);
    connect(ui.pb_tcp_init, &QPushButton::clicked, this, &clientform::tcpInit);
 
    connect(ui.rb_tcp, &QRadioButton::toggled, this, &clientform::radioBtnTCPSlot);
    connect(ui.rb_udp_one, &QRadioButton::toggled, this, &clientform::radioBtnUDPOneSlot);
    connect(ui.rb_udp_group, &QRadioButton::toggled, this, &clientform::radioBtnUDPMutilSlot);
    connect(ui.rb_udp_broadcast, &QRadioButton::toggled, this, &clientform::radioBtnUDPBroadCastSlot);
     
    group_type = new QButtonGroup();
    group_type->addButton(ui.rb_tcp, 1);
    group_type->addButton(ui.rb_udp_one, 2);
    group_type->addButton(ui.rb_udp_group, 3);
    group_type->addButton(ui.rb_udp_broadcast, 4);

    choose(2);
}

void clientform::radioBtnTCPSlot()
{
    if (ui.rb_tcp->isChecked())
    {
        choose(1);
    }
}
void clientform::radioBtnUDPOneSlot()
{
    if (ui.rb_udp_one->isChecked())
    {
        choose(2);
    }
}
void clientform::radioBtnUDPMutilSlot()
{
    if (ui.rb_udp_group->isChecked())
    {
        choose(3);
    }
}
void clientform::radioBtnUDPBroadCastSlot()
{
    if (ui.rb_udp_broadcast->isChecked())
    {
        choose(4);
    }
}

void clientform::choose(int type)
{
    ui.le_server_ip->setEnabled(false);
    ui.le_server_port->setEnabled(false);
    ui.le_receive_port->setEnabled(false);
    ui.pb_init->setEnabled(false);
    ui.pb_remove->setEnabled(false);

    ui.le_group_ip->setEnabled(false);
    ui.le_group_port->setEnabled(false);
    ui.pb_group_init->setEnabled(false);
    ui.pb_group_remove->setEnabled(false);

    ui.le_tcpserver_ip->setEnabled(false);
    ui.le_tcpsever_port->setEnabled(false);
    ui.pb_tcp_init->setEnabled(false);
    ui.pb_tcp_remove->setEnabled(false);

    if (type == 2 || type == 4)
    {
        ui.le_server_ip->setEnabled(true);
        ui.le_server_port->setEnabled(true);
        ui.le_receive_port->setEnabled(true);
        ui.pb_init->setEnabled(true);
        ui.pb_remove->setEnabled(true);
    }

    if (type == 1)
    {
        ui.le_tcpserver_ip->setEnabled(true);
        ui.le_tcpsever_port->setEnabled(true);
        ui.pb_tcp_init->setEnabled(true);
        ui.pb_tcp_remove->setEnabled(true);
    }

    if (type == 3)
    {
        ui.le_group_ip->setEnabled(true);
        ui.le_group_port->setEnabled(true);
        ui.pb_group_init->setEnabled(true);
        ui.pb_group_remove->setEnabled(true);
    }
}

clientform::~clientform()
{
    if (m_udp_one_handler)
    {
        delete m_udp_one_handler;
        m_udp_one_handler = nullptr;
    }
}

void clientform::init()
{
    QString server_ip = ui.le_server_ip->text();
    int server_port = ui.le_server_port->text().toInt();
    int receive_port = ui.le_receive_port->text().toInt();
    m_udp_one_handler = new UDPOneHandler(server_ip, server_port, receive_port); 
    connect(m_udp_one_handler, &UDPOneHandler::displayInfo, this, &clientform::displayInfo);
    connect(this, &clientform::sendMsg, m_udp_one_handler, &UDPOneHandler::sendData);
    connect(ui.pb_remove, &QPushButton::clicked, this, &clientform::disconnectUdp);
    m_udp_one_handler->init();
}

void clientform::disconnectUdp()
{
    if (m_udp_one_handler)
    {
        delete m_udp_one_handler;
        m_udp_one_handler = nullptr;
    }
}

void clientform::sendData()
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

void clientform::tcpDisconnect()
{
    //if (m_tcp_client_handler != nullptr)
    //{
    //    m_tcp_client_handler->disconnectTcp();
    //}
    delete m_tcp_client_handler;
    m_tcp_client_handler = nullptr;
    disconnect(ui.pb_tcp_remove, &QPushButton::clicked, this, &clientform::tcpDisconnect);
}

void clientform::tcpInit()
{
    QString _ip = ui.le_tcpserver_ip->text();
    int _port = ui.le_tcpsever_port->text().toInt();
    m_tcp_client_handler = new TCPClientHandler(_ip, _port);
    connect(m_tcp_client_handler, &TCPClientHandler::displayInfo, this, &clientform::displayInfo);
    connect(this, &clientform::sendTcpMsg, m_tcp_client_handler, &TCPClientHandler::sendData);
    connect(ui.pb_tcp_remove, &QPushButton::clicked, this, &clientform::tcpDisconnect);
    m_tcp_client_handler->init();
}

void clientform::groupInit()
{
    QString _ip = ui.le_group_ip->text();
    int _port = ui.le_group_port->text().toInt();
    m_udp_mutilcast_handler = new UDPMutilcastHandler(_ip, _port);
    connect(m_udp_mutilcast_handler, &UDPMutilcastHandler::displayInfo, this, &clientform::displayInfo);
    connect(this, &clientform::sendGroupMsg, m_udp_mutilcast_handler, &UDPMutilcastHandler::sendData);
    connect(ui.pb_group_remove, &QPushButton::clicked, this, &clientform::disconnectMutilUDP);
    m_udp_mutilcast_handler->init();
}

void clientform::disconnectMutilUDP()
{
    if (m_udp_mutilcast_handler)
    {
        delete m_udp_mutilcast_handler;
        m_udp_mutilcast_handler = nullptr;
    }
}
void clientform::sendGroupData()
{
    QString info = ui.te_send_info->toPlainText();
    emit sendGroupMsg(info);
    displayInfo(false, "自己:" + info);
}

void clientform::displayInfo(bool is_alarm, QString info)
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

QString clientform::getLocalIP()
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