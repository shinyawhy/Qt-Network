#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();

private:
    Ui::Server *ui;
    QTcpServer *tcpServer;

private slots:
    void sendMessage();
};
#endif // SERVER_H
