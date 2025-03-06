#ifndef GAMETCPCLIENT_H
#define GAMETCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "AGameController.h"
#include <QDataStream>

//реализация абстрактного контроллера для взаимодействия с оппонентом по протоколу TCP
//а также с виджетом игры
class TcpController : public AGameController
{
    Q_OBJECT

protected:
    QTcpSocket* socket; //TCP сокет, через который происходит обмен данными с оппонентом

    //перечисление всех типов данных, которые можно принять или отправить
    enum DataTypes: quint8
    {
        Ready,
        Code,
        Key,
        Name
    };

    QDataStream stream; //поток для чтения сокета
    quint8 messageSize = 0; //хранит размер следующего сообщения для правильного разделения сообщений
    void ReadMessage(const QByteArray& message); //читать сообщение уже принятое через сокет и реагировать
    void ReadSocket(); //метод для чтения сокета и отделения сообщений
    void SendMessage(const QByteArray& data); //отправить сообщение через сокет с присоединением заголовка с размером

public:
    explicit TcpController(QTcpSocket* socket, QObject *parent = nullptr);

public slots:
    //реализуем абстрактные методы базового класса
    void SendReady();
    void SendCode(quint64 code);
    void SendKey(const QByteArray& keyArr);
    void SetAction(quint64 action);
    void SetReady();

    void SendName(const QString& name); //передаем имя, чтобы оппонент знал, с кем имеет дело

signals:
    void nameReceived(const QString& name); //получено и мя оппонента
    void disconnected(); //соединение потеряно
};

#endif // GAMETCPCLIENT_H
