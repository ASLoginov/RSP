#include "GameBrowser.h"

GameBrowser::GameBrowser(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &GameBrowser::ProcessUdpDatagrams);
}

//для поиска игры отправляем широковещательно FIND_GAMES и ждем ответа от сервера
void GameBrowser::SearchGames() {
    QByteArray datagram = "FIND_GAMES";
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 45000);
}

void GameBrowser::ProcessUdpDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        //нужная нам датаграмма имеет такую структуру: GAME_INFO\n<имя>\n<адрес>\n<порт>
        if (datagram.startsWith("GAME_INFO")) {
            QStringList parts = QString(datagram).split('\n');
            if (parts.size() == 4) {
                emit gameFound(parts[1], parts[2], parts[3].toUShort());
            }
        }
    }
}
