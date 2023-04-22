#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString localHostName = QHostInfo::localHostName();     // 获取本地主机名称
    QHostInfo info = QHostInfo::fromName(localHostName);    // 根据主机名获取QHostInfo对象
    qDebug() << "localHostName: " << localHostName << endl << "IP Address: " << info.addresses();

    // info.addresses()返回一个QHostAddress对象的列表(从一个主机名获取的IP地址可能不止一个)
    foreach(QHostAddress address, info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            qDebug() << address.toString();
    }

    QHostInfo::lookupHost("www.baidu.com", this, SLOT(lookedUp(QHostInfo)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lookedUp(const QHostInfo &host)
{
    if(host.error() != QHostInfo::NoError) {	// 返回错误信息
            qDebug() << "Lookup failed: " << host.errorString();
            return;
        }
        // 遍历IP列表并输出
        foreach(const QHostAddress &address, host.addresses())
            qDebug() << "Found address: " << address.toString();

}

