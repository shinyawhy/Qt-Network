#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QNetworkReply;
class QNetworkAccessManager;
class QFile;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startRequest(QUrl url);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *manager;
    QNetworkReply         *reply;
    QUrl                  url;
    QFile                 *file;

private slots:
    void replyFinished(QNetworkReply *reply);

    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);


    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
