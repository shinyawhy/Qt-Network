#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QDialog
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private:
    Ui::Client *ui;
    QTcpSocket *tcpSocket;
    QString message;
    quint16 blockSize;
private slots:
    void newConnect();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void on_pushButton_clicked();
};
#endif // CLIENT_H
