#include "udpreceiver.h"

udpReceiver::udpReceiver(QWidget *parent) : QDialog(parent)
{
    receiver = new QUdpSocket(this);
    // 先使用bind()绑定发送信息的IP地址和端口号
    // QUdpSocket::ShareAddress是绑定模式，这个模式表示允许其他服务器绑定到相同的地址和端口上
    receiver->bind(45454, QUdpSocket::ShareAddress);

    // 每当有数据报到来时，QUdpSocket都会发射readyRead()信号
    connect(receiver, &QUdpSocket::readyRead, this, &udpReceiver::processPendingDatagram);


    rcLabel = new QLabel("等待接受数据", this);
    rcLabel->setGeometry(50, 50, 100, 30);

}

void udpReceiver::processPendingDatagram()
{
    // 持续检查是否有数据报发送过来
        while(receiver->hasPendingDatagrams())
        {
            QByteArray datagram;
            // 让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
            datagram.resize(receiver->pendingDatagramSize());	// pendingDatagramSize()获取当前数据报的大小

            // 接收数据报，将其存放到datagram中(使用readDatagram()接收不大于指定大小的数据报，并将其存储到QByteArray变量中)
            receiver->readDatagram(datagram.data(), datagram.size());
            rcLabel->setText(datagram);	// 将数据报的内容显示在label部件中
        }

}
