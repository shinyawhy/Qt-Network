#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QDialog>
#include <QUdpSocket>
#include <QPushButton>

class udpSender : public QDialog
{
public:
    udpSender(QWidget *parent = nullptr);

private:
    QUdpSocket *sender;

private slots:
    void on_bcBtn_clicked();
};

#endif // UDPSENDER_H
