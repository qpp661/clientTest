#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class Widget;
}
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_sendPushButton_clicked();

    void on_disconectPushButton_clicked();

    void on_connectPushButton_clicked();
    void connectSuccess();
    void handlerReceive();
    void disconnectSuccess();

private:
    Ui::Widget *ui;
    QTcpSocket * tcpSocket; //通信套接字对象
    QString userName;

};

#endif // WIDGET_H
