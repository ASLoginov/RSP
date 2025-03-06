#include "MainWindow.h"
#include "Controllers/TcpController.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    //предлагаем ввести имя игрока
    QString input = QInputDialog::getText(this, "Hello!", "Ener your name:");
    name = input.length() ? input : name;

    //создаем и показываем браузер, при подключении запускаем игру
    browser = new BrowserWidget(name);
    connect(browser, &BrowserWidget::connected, this, &MainWindow::StarGame);
    browser->show();
}

//переход из браузера в игру
void MainWindow::StarGame(QTcpSocket* socket)
{
    //прячем браузер
    browser->hide();
    //создаеем виджет игры
    game = new GameWidget();
    //создаем и подключаем контроллер
    TcpController* controller = new TcpController(socket);
    game->SetController(controller);
    //инициализируем игровую механику
    Round* round = new Round(new ExponentialMachine());
    //подключаем контроллер
    round->SetController(controller);
    //подключаем раунд к виджету игры
    game->SetRound(round);
    //прописываем имя игрока и отправляем его оппоненту
    game->SetP1Name(name);
    controller->SendName(name);
    //готовим пути отступления
    connect(game, &GameWidget::quite, this, &MainWindow::Restart);
    //заголовочек для красоты
    game->setWindowTitle(name);
    //все готово, показываем виджет игры
    game->show();
}

//перезапуск, снова видим браузер и можем начинать новую игру
void MainWindow::Restart()
{
    game->deleteLater();
    browser->deleteLater();

    browser = new BrowserWidget(name);
    connect(browser, &BrowserWidget::connected, this, &MainWindow::StarGame);
    browser->show();
}

