#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QUdpSocket>
#include <QDialog>
#include <QLabel>

class udpReceiver : public QDialog
{
public:
    udpReceiver(QWidget *parent = nullptr);

private:
    QUdpSocket *receiver;
    QLabel *rcLabel;

private slots:
    void processPendingDatagram();
};

#endif // UDPRECEIVER_H
