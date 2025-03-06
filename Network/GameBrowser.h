#ifndef GAMEBROWSER_H
#define GAMEBROWSER_H

#include <QUdpSocket>
#include <QTcpSocket>

//этот класс инкапсулирует логику поиска игры в локальной сети
//для обнаружения используем широковещательные пакеты по протоколу UDP
//при получении такого пакета сервер должен отправить нам адрес для подключения по TCP
class GameBrowser : public QObject {
    Q_OBJECT

    QUdpSocket *udpSocket;

private slots:
    void ProcessUdpDatagrams(); //здесь происходит обработка полученной датаграммы

public:
    explicit GameBrowser(QObject *parent = nullptr);
    void SearchGames(); //собственно, метод поиска

signals:
    void gameFound(const QString& name, const QString& address, quint16 port); //нашли игру :)
};

#endif // GAMEBROWSER_H
