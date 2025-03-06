#include "GameServer.h"
#include <QNetworkInterface>
#include <QTcpSocket>

GameServer::GameServer(const QString& playerName, quint16 port, QObject *parent) : QObject(parent)
{
    tcpPort = port;
    name = playerName;

    //запускаем TCP сервер
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, tcpPort))
    {
        qDebug() << "Ошибка запуска сервера:" << tcpServer->errorString();
        return;
    }
    address = GetLocalIPv4Address(); //определяем наш адрес в сети
    connect(tcpServer, &QTcpServer::newConnection, this, &GameServer::NewConnection);

    // настраиваем UDP сокет для обнаружения
    udpSocket = new QUdpSocket(this);
    if (!udpSocket->bind(45000, QUdpSocket::ShareAddress))
    {
        qDebug() << "Ошибка привязки UDP:" << udpSocket->errorString();
        return;
    }
    connect(udpSocket, &QUdpSocket::readyRead, this, &GameServer::ProcessUdpDatagrams);
}

const QString &GameServer::Address()
{
    return address;
}

quint16 GameServer::Port()
{
    return tcpPort;
}


void GameServer::NewConnection()
{
    QTcpSocket* socket = tcpServer->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    emit successfullConnection(socket);
}

//определяем локальный адрес
QString GameServer::GetLocalIPv4Address()
{
    //ищем среди доступных сетевых интерфейсов что-то похожее на типичный адрес в локальной сети
    foreach(const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
            && address != QHostAddress(QHostAddress::LocalHost)
            && address.toString().contains("192.168."))
        {
            return address.toString();
        }
    }
    return QHostAddress(QHostAddress::LocalHost).toString(); //если ничего похожего не нашли возвращаем локалхост
}

//обрабатываем полученную датаграмму
void GameServer::ProcessUdpDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        //если датаграмма - это сообщение о поиске игры, то отправляем в ответ имя, адрес и порт
        if (datagram == "FIND_GAMES")
        {
            QByteArray response = QString("GAME_INFO\n%1\n%2\n%3")
                    .arg(name, address)
                    .arg(tcpPort)
                    .toUtf8();

            udpSocket->writeDatagram(response, sender, senderPort);
        }
    }
}
