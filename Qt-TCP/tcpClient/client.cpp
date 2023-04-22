#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readMessage);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

}

Client::~Client()
{
    delete ui;
}

void Client::newConnect()
{
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostlineEdit->text(), ui->portlineEdit->text().toInt());
}

void Client::readMessage()
{
    QDataStream in(tcpSocket);	// 定义输入流
    // 设置数据流版本，这个要和服务器端的相同
    in.setVersion(QDataStream::Qt_5_5);
    // 如果是刚开始就接收数据
    if(blockSize == 0) {
        // 判断接收的数据是否大于两字节，也就是文件的大小信息所占的空间
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }
    // 如果没有得到全部的数据，则返回，继续接收数据
    if(tcpSocket->bytesAvailable() < blockSize)
        return;
    in >> message;	// 将接收到的数据存放到变量中
    ui->messagelabel->setText(message);	// 在label中显示接收到的数据
}

void Client::displayError(QAbstractSocket::SocketError)
{
    qDebug()<< tcpSocket->errorString();
}


void Client::on_pushButton_clicked()
{
    newConnect();
}
