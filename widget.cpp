#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Client");
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connectSuccess()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(handlerReceive()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnectSuccess()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::connectSuccess(){
    ui->receiveTextBrowser->setText("和服务器建立连接");
    QJsonObject json;
    json.insert("type",1);
    json.insert("to",0);
    json.insert("from",userName);
    QJsonDocument doc;
    doc.setObject(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QByteArray sendByte;
    QDataStream out(&sendByte,QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out<<qint16(0)<<qint16(0)<<data;

    out.device()->seek(0);
    qint16 type_id = 0;
    qint16 jsonLen = ((sendByte.size() - sizeof(qint16)*2));
    out<<type_id<<jsonLen;
    tcpSocket->write(sendByte);
}
void Widget::handlerReceive(){
    QByteArray array = tcpSocket->readAll();
    ui->receiveTextBrowser->append(array);
}

void Widget::on_sendPushButton_clicked()
{   if(ui->SendPlainTextEdit->toPlainText().isEmpty()){
        QMessageBox::information(Q_NULLPTR, QString("注意"), QString("发送内容不能为空！"),QMessageBox::Yes);
        return;
    }
    QString str = ui->SendPlainTextEdit->toPlainText();
    int friId = ui->comunicateLineEdit->text().toInt();
    QJsonObject json;
    json.insert("type",2);
    json.insert("to",friId);
    json.insert("from",userName);
    json.insert("message",str);
    QJsonDocument doc;
    doc.setObject(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QByteArray sendByte;
    QDataStream out(&sendByte,QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out<<qint16(0)<<qint16(0)<<data;

    out.device()->seek(0);
    qint16 type_id = 1;
    qint16 jsonLen = ((sendByte.size() - sizeof(qint16)*2));
    out<<type_id<<jsonLen;

    tcpSocket->write(sendByte);
    ui->SendPlainTextEdit->clear();
}

void Widget::disconnectSuccess(){
    ui->receiveTextBrowser->setText("断开连接");
}



void Widget::on_disconectPushButton_clicked()
{
    ui->receiveTextBrowser->setText("连接断开");
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}


void Widget::on_connectPushButton_clicked()
{
    if(ui->portLineEdit->text().isEmpty()||ui->ipLineEdit->text().isEmpty()||ui->userLineEdit->text().isEmpty()){
        QMessageBox::information(Q_NULLPTR,QString("注意"),QString("port,ip,userId不能为空!"),QMessageBox::Yes);
        return;
    }
    QString ipSer = ui->ipLineEdit->text();
    quint16 portSer = ui->portLineEdit->text().toUShort();
    userName = ui->userLineEdit->text();
    tcpSocket->connectToHost(QHostAddress(ipSer),portSer);

}



