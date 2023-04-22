#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    payloadSize = 64 * 1024;	// 64kb
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket(this);
    // 连接服务器成功，会发出connect()信号，开始传送文件
    connect(tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
    connect(tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    ui->sendButton->setEnabled(false);

}

Client::~Client()
{
    delete ui;
}

void Client::openFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()) {
        ui->sendButton->setEnabled(true);
        ui->clientStatusLabel->setText(tr("打开文件 %1 成功!").arg(fileName));
    }
}

void Client::send()
{
    ui->sendButton->setEnabled(false);
    // 初始化已发送字节为0
    bytesWritten = 0;
    ui->clientStatusLabel->setText(tr("连接中..."));
    // 使用connectHost()来连接服务器
    tcpClient->connectToHost(ui->hostLineEdit->text(), ui->portLineEdit->text().toInt());
}


void Client::startTransfer()
{
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly)) {
        qDebug() << "client: open file error!";
        return;
    }
    // 获取文件大小
    totalBytes = localFile->size();	// 这里totalBytes表示待发送数据的总大小(包括文件结构和实际文件的大小)
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_5);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf("/") - 1);
    // 保留总大小信息空间，文件名大小信息空间，然后输入文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;
    // 这里的总大小是总大小信息，文件名大小信息，文件名和实际文件大小的总和
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    // 使用seek(0)返回outBlock的开始，用实际的大小信息代替前面占位的qint64()的两个空间
    sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64) * 2));
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    ui->clientStatusLabel->setText(tr("已连接"));
    outBlock.resize(0);	// outBlock只是一个用于暂存数据的缓冲区数组，最后要将其清空
}


void Client::updateClientProgress(qint64 numBytes)
{
    // 已经发送数据的大小
    bytesWritten += (int)numBytes;
    // 如果已经发送了数据
    if(bytesToWrite > 0) {
        // 每次发送payloadSize大小的数据，这里设置为64kb，如果剩余的数据不足64kb，那么就发送剩余数据的大小
        outBlock = localFile->read(qMin(bytesToWrite, payloadSize));
        // 发送完一次数据后还剩余数据的大小
        bytesToWrite -= (int)tcpClient->write(outBlock);
        // 清空发送缓冲区
        outBlock.resize(0);
    } else {
        localFile->close();
    }

    // 更新进度条最大值和当前值
    ui->clientProgressBar->setMaximum(totalBytes);
    ui->clientProgressBar->setValue(bytesWritten);
    // 如果发送完毕
    if(bytesWritten == totalBytes) {
        ui->clientStatusLabel->setText(tr("传送 %1 成功").arg(fileName));
        localFile->close();
        tcpClient->close();
    }
}


void Client::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(tr("客户端就绪"));
    ui->sendButton->setEnabled(true);
}

void Client::on_openButton_clicked()
{
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(tr("状态: 等待打开文件!"));
    openFile();
}

void Client::on_sendButton_clicked()
{
    send();
}
