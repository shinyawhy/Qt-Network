#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);
    // QHostAddress::LocalHost表示Ipv4的本地主机地址，等价于QHostAddress("127.0.0.1")，使用本地主机可以使客户端和服务器端在同一台计算机上进行通信
    // 使用listen()监听从这个端口到来的连接
    if(!tcpServer->listen(QHostAddress::LocalHost, 6666)) {
        qDebug() << tcpServer->errorString();
        close();
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendMessage);
}

Server::~Server()
{
    delete ui;
}

void Server::sendMessage()
{
    // QByteArray暂存数据
    QByteArray block;
    // 使用QDatastream数据流将要发送的数据写入block中
    QDataStream out(&block, QIODevice::WriteOnly);
    // 设置数据流版本
    out.setVersion(QDataStream::Qt_5_5);
    // 数据流最开始的两个字节要用于填写待发送数据的大小
    out << (quint16)0; // 强制类型转换（quint16占两字节，这里用于占位
    out << tr("hello tcp");
    out.device()->seek(0);  // 写入指针跳转到数据流开头， 下面想数据流的开头写入数据大小信息
    out << (quint16)(block.size()) - sizeof(quint16); // 总大小 - 开头大小

    // 获取已经建立的套接字对象
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

    // 当连接断开时删除该套接字
    connect(clientConnection, &QTcpSocket::disconnected, clientConnection, &QTcpSocket::deleteLater);
    clientConnection->write(block);
    clientConnection->disconnectFromHost(); // disconnectFromHost会一直等待套接字将所有数据发送完毕，然后关闭套接字

    ui->label->setText(tr("sucess!"));
}

