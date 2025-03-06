#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QUdpSocket>

//этот класс предоставляет возможность подключиться по локальной сети
class GameServer : public QObject {
    Q_OBJECT

    QTcpServer *tcpServer; //TCP сервер будет слушать определенный порт и ждать подключения
    QUdpSocket *udpSocket; //используем для ответа ищущим игру
    quint16 tcpPort; //этот порт слушаем
    QString name; //имя игрока, создавшего игру - будет отправлено в ответе на поиск игры
    QString address; //адрес, по которому нужно будет подключаться по TCP

    QString GetLocalIPv4Address(); //здесь выясняем наш адрес в локальной сети
public:
    explicit GameServer(const QString& name, quint16 port, QObject *parent = nullptr);
    const QString& Address(); //геттер для адреса
    quint16 Port(); //геттер для порта

private slots:
    void NewConnection(); //когда установлено соединение
    void ProcessUdpDatagrams(); //обработка от ищущих игру

signals:
    void successfullConnection(QTcpSocket* tcpSocket); //успешно подключенный сокет
};
#endif // GAMESERVER_H
