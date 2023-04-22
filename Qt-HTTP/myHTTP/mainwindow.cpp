#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 实例化对象
    manager = new QNetworkAccessManager(this);

    ui->progressBar->hide();


    ui->lineEdit->setPlaceholderText("www.***.com");

    // 每次网络应答结束后发送这个信号
//    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished);

    // 发送一个网络请求
    manager->get(QNetworkRequest(QUrl("http://www.baidu.com")));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startRequest(QUrl url)
{
    reply = manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::readyRead, this, &MainWindow::httpReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &MainWindow::updateDataReadProgress);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::httpFinished);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    QString all = reply->readAll();

    ui->textBrowser->setText(all);

    reply->deleteLater();
}

void MainWindow::httpFinished()
{
//    ui->progressBar->hide();
    if(file)
    {
        file->close();
        delete  file;
        file = 0;
    }
    reply->deleteLater();
    reply = 0;
}

void MainWindow::httpReadyRead()
{
    if(file)
        file->write(reply->readAll());
}

void MainWindow::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}


void MainWindow::on_pushButton_clicked()
{
    url = ui->lineEdit->text();
    QFileInfo info(url.path());
    QString fileName(info.fileName());
    // 保存GET请求得到的网页内容
    if(fileName.isEmpty())
        fileName = "index.html";
    file = new QFile(fileName);
    if(!file->open(QIODevice::WriteOnly))
    {
        delete  file;
        file = 0;
        return ;
    }
    startRequest(url);
    ui->progressBar->setValue(0);
    ui->progressBar->show();

}
