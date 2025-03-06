#include "TcpController.h"

//инициализируем все
TcpController::TcpController(QTcpSocket* socket, QObject *parent) :
    AGameController(parent), socket(socket), stream(socket)
{
    stream.setVersion(QDataStream::Qt_6_8);
    //когда сокет сигнализирует, что есть, что прочитать, переходим к процессу чтения
    connect(socket, &QTcpSocket::readyRead, this, &TcpController::ReadSocket);
    //при дисконнекте просто шлем сигнал
    connect(socket, &QTcpSocket::disconnected, this, [this]() { emit disconnected(); });
}

//отправляем сообщение о готовности
void TcpController::SendReady()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_8);
    stream << Ready;
    SendMessage(data);
    qDebug() << "sending Ready";
}

//отправляем шифр
void TcpController::SendCode(quint64 code)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_8);
    stream << Code << code;
    SendMessage(data);
    qDebug() << "sending Code";
}

//отправляем ключ
void TcpController::SendKey(const QByteArray &keyArr)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_8);
    stream << Key << keyArr;
    SendMessage(data);
    qDebug() << "sending Key";
}

//отправляем имя
void TcpController::SendName(const QString &name)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_8);
    stream << Name << name;
    SendMessage(data);
    qDebug() << "sending Name";
}

void TcpController::SetAction(quint64 action)
{
    emit actionSet(action);
}

void TcpController::SetReady()
{
    emit readySet();
}

//разбираем полученное сообщение и отправляем соответствующий сигнал
void TcpController::ReadMessage(const QByteArray& message)
{
    QDataStream stream(message);
    stream.setVersion(QDataStream::Qt_6_8);
    quint8 dataType;
    stream >> dataType;
    switch (dataType)
    {
    case Ready:
    {
        qDebug() << "read Ready";
        emit readyReceived();
        break;
    }
    case Code:
    {
        quint64 code;
        stream >> code;
        qDebug() << "read Code";
        emit codeReceived(code);
        break;
    }
    case Key:
    {
        QByteArray key;
        stream >> key;
        qDebug() << "read Key";
        emit keyReceived(key);
        break;
    }
    case Name:
    {
        QString name;
        stream >> name;
        qDebug() << "read Ready";
        emit nameReceived(name);
    }
        break;
    default:
        break;
    }
}

//читаем данные из сокета
//структура данных такая: сначала 1 байт с размером следующедо, затем собственно сообщение
void TcpController::ReadSocket()
{
    while (true)
    {
        if (messageSize == 0) //еще на прочли заголовок с размером
        {
            if (socket->bytesAvailable() < sizeof(messageSize)) return; //недостаточно байт для чтения заголовка
            //а вот если достаточно, то читаем
            stream >> messageSize;
        }

        if (socket->bytesAvailable() < messageSize) return; //еще не можем прочитать все сообщение

        //а вот теперь можем
        QByteArray data;
        stream >> data; //сообщение сохранили
        messageSize = 0; //подготовились читать дальше

        qDebug() << "Received:" << data;
        ReadMessage(data); //и отправляем сообщение на разбор
    }
}

//метод, в котором присоединяем к сообщению заголовок с размером и отправляем
void TcpController::SendMessage(const QByteArray &data)
{
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_8);
    stream << quint8(0) << data; // временный заголовок (0)
    stream.device()->seek(0);
    stream << quint8(block.size() - sizeof(quint8)); // записываем реальный размер
    socket->write(block);
}
