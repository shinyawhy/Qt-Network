#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udpsender.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpSender *Senderdialog = new udpSender(this);
    Senderdialog->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

