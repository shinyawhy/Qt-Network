#include "udpsender.h"

udpSender::udpSender(QWidget *parent) : QDialog(parent)
{
    sender = new QUdpSocket(this);

    auto bcBtn = new QPushButton("broadcast", this);
    bcBtn->setGeometry(50, 50, 100, 30);
    connect(bcBtn, &QPushButton::clicked, this, &udpSender::on_bcBtn_clicked);
}

void udpSender::on_bcBtn_clicked()
{
    QByteArray datagram = "hello world!";
    // QHostAddress::Broadcast表示广播，向所有主机发送
    sender->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);
}

