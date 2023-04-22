#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udpreceiver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpReceiver *Receivedialog = new udpReceiver(this);
    Receivedialog->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

