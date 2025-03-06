#include "BrowserWidget.h"
#include <QBoxLayout>

//обрабатываем кнопку создания игры
void BrowserWidget::HandleCreateButton(bool checked)
{
    if (checked)
    {
        //блокируем ненужные элементы
        games->setEnabled(false);
        joinButton->setEnabled(false);
        findButton->setEnabled(false);
        address->setEnabled(false);
        port->setEnabled(false);

        //если сервера еще нет, то создаем его на нужном порту
        if (!server)
        {
            server = new GameServer(name, port->text().toUShort(), this);
            //при успешном подключении сигнализируем
            connect(server, &GameServer::successfullConnection, this, [this](QTcpSocket* socket) { emit connected(socket); });
        }
        //показываем адрес сервера на всякий случай
        addressInfo->setText("Server address is " + server->Address() + ":" + QString::number(server->Port()));
    }
    else
    {
        //разблокируем интерфейс и удаляем сервер
        games->setEnabled(true);
        joinButton->setEnabled(true);
        findButton->setEnabled(true);
        address->setEnabled(true);
        port->setEnabled(true);
        server->deleteLater();
        server = nullptr;
        addressInfo->setText("");
    }
}

//обрабатываем кнопку поиска
void BrowserWidget::HandleFindButton()
{
    //очищаем список
    games->setCurrentItem(nullptr);
    games->clear();
    if (!browser)
    {
        browser = new GameBrowser();
        //найденные игры добавляем в список
        connect(browser, &GameBrowser::gameFound, this, [this](const QString& name, const QString& address, quint16 port) {
            games->addItem(name + " " + address + ":" + QString::number(port));
        });
    }
    //запускаем поиск
    browser->SearchGames();
}

//создаем сокет и пытаемся подключиться по выбранному адресу
void BrowserWidget::JoinGame()
{
    if (!socket)
    {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, [this]() { emit connected(socket); });
    }
    QStringList host = address->text().split(":");
    if (host.size() == 2) socket->connectToHost(host[0], host[1].toUShort());
}

BrowserWidget::BrowserWidget(const QString& playerName, QWidget *parent) : QWidget(parent)
{
    //создаем и настраиваем интерфейс
    name = playerName;
    setWindowTitle(name);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    games = new QListWidget();
    vLayout->addWidget(games);

    QHBoxLayout* addressLayout = new QHBoxLayout();
    address = new QLineEdit("Enter address:port");
    addressLayout->addWidget(address);
    joinButton = new QPushButton("Join");
    addressLayout->addWidget(joinButton);
    vLayout->addLayout(addressLayout);

    QHBoxLayout* selectLayout = new QHBoxLayout();
    createButton = new QPushButton("Create game");
    createButton->setCheckable(true);
    findButton = new QPushButton("Find game");
    portLabel = new QLabel("Port:");
    port = new QLineEdit("45001");
    selectLayout->addWidget(portLabel);
    selectLayout->addWidget(port);
    selectLayout->addWidget(createButton);
    selectLayout->addWidget(findButton);
    vLayout->addLayout(selectLayout);
    addressInfo = new QLabel();
    addressInfo->setTextInteractionFlags(Qt::TextSelectableByMouse);
    vLayout->addWidget(addressInfo);

    connect(games, &QListWidget::currentItemChanged, this, [this](QListWidgetItem* item) {
        if (item)
        {
            QStringList list = item->text().split(" ");
            address->setText(list[list.size() - 1]);
        }
        else address->setText("");
    });
    connect(findButton, &QPushButton::clicked, this, &BrowserWidget::HandleFindButton);
    connect(createButton, &QPushButton::clicked, this, &BrowserWidget::HandleCreateButton);
    connect(joinButton, &QPushButton::clicked, this, &BrowserWidget::JoinGame);
}
