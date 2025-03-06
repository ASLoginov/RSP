#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H

#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "Network/GameBrowser.h"
#include "Network/GameServer.h"

//виджет для управления процессом поиска и создания игры
class BrowserWidget : public QWidget
{
    Q_OBJECT

    QListWidget* games; //список найденных игр
    QLineEdit* address; //ввод адреса для подключения
    QPushButton* joinButton; //кнопка подключения по адресу в address
    QLabel* portLabel;
    QLineEdit* port; //порт, на котором создается игра
    QPushButton* findButton; //кнопка запуска поиска игр
    QPushButton* createButton; //кнопка создания игры и ожидания подключений
    QLabel* addressInfo; //адрес, по которому можно подключиться к созданной игре по локальной сети

    GameServer* server = nullptr; //чтобы создавать игру
    GameBrowser* browser = nullptr; //чтобы искать игру
    QTcpSocket* socket = nullptr; //используется для подключения к найденной игре

    QString name; //имя игрока

    void HandleCreateButton(bool checked); //обработка нажатия кнопки создания
    void HandleFindButton(); //обработка нажатия кнопки поиска

private slots:
    void JoinGame(); //подключиться к игре

public:
    explicit BrowserWidget(const QString& playerName, QWidget *parent = nullptr);

signals:
    void connected(QTcpSocket* socket); //установлено соединение по TCP
};

#endif // BROWSERWIDGET_H
