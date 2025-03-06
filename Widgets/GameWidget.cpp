#include "GameWidget.h"
#include "RoundWidget.h"
#include "Utility/Utility.h"
#include <QBoxLayout>


GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    //создание и настройка интерфейса
    resize(400, 600);
    QVBoxLayout* gameField = new QVBoxLayout(this);

    QHBoxLayout* quiteLayout = new QHBoxLayout();
    quiteLayout->addStretch();
    quiteButton = new QPushButton("Quite");
    quiteLayout->addWidget(quiteButton);
    gameField->addLayout(quiteLayout);

    history = new QListWidget();
    gameField->addWidget(history);

    QHBoxLayout* p2Info = new QHBoxLayout();
    p2Name = new QLabel("Player2: ");
    p2Won = new QLabel("0");
    p2Info->addWidget(p2Name);
    p2Info->addWidget(p2Won);
    p2Info->addStretch();
    gameField->addLayout(p2Info);

    p2Action = new QLabel();
    p2Action->setFixedHeight(80);
    gameField->addWidget(p2Action);
    p2Action->setAlignment(Qt::AlignHCenter);

    roundResult = new QLabel();
    roundResult->setFixedHeight(80);
    SetResultToLabel(roundResult, 3, 70);
    gameField->addWidget(roundResult);
    roundResult->setAlignment(Qt::AlignCenter);

    p1Action = new QLabel();
    p1Action->setFixedHeight(80);
    gameField->addWidget(p1Action);
    p1Action->setAlignment(Qt::AlignHCenter);

    QHBoxLayout* p1Info = new QHBoxLayout();
    p1Name = new QLabel("Player1: ");
    p1Won = new QLabel("0");
    p1Info->addWidget(p1Name);
    p1Info->addWidget(p1Won);
    p1Info->addStretch();
    gameField->addLayout(p1Info);

    roundNumber = new QLabel("0");
    QHBoxLayout* roundInfo = new QHBoxLayout();
    roundInfo->addStretch();
    roundInfo->addWidget(new QLabel("Round "));
    roundInfo->addWidget(roundNumber);
    roundInfo->addStretch();
    gameField -> addLayout(roundInfo);

    QHBoxLayout* actionButtons = new QHBoxLayout();
    rockButton = new QPushButton("Rock");
    rockButton->setIcon(QIcon(":/ico/ico/rock.png"));
    rockButton->setEnabled(false);
    scissorsButton = new QPushButton("Scissors");
    scissorsButton->setIcon(QIcon(":/ico/ico/scissors.png"));
    scissorsButton->setEnabled(false);
    paperButton = new QPushButton("Paper");
    paperButton->setIcon(QIcon(":/ico/ico/paper.png"));
    paperButton->setEnabled(false);
    actionButtons->addWidget(rockButton);
    actionButtons->addWidget(scissorsButton);
    actionButtons->addWidget(paperButton);
    QVBoxLayout* buttons = new QVBoxLayout();
    buttons->addLayout(actionButtons);
    newRoundButton = new QPushButton("New round");
    buttons->addWidget(newRoundButton);
    gameField->addLayout(buttons);

    connect(quiteButton, &QPushButton::clicked, this, &GameWidget::Quite);
    connect(newRoundButton, &QPushButton::clicked, this, &GameWidget::HandleNewRoundButton);
    connect(rockButton, &QPushButton::clicked, this, [this]() { SelectAction(rockButton, 1); });
    connect(scissorsButton, &QPushButton::clicked, this, [this]() { SelectAction(scissorsButton, 2); });
    connect(paperButton, &QPushButton::clicked, this, [this]() { SelectAction(paperButton, 3); });
}

//подключение игровой механики
void GameWidget::SetRound(Round* roundObject)
{
    round = roundObject;
    round->setParent(this);

    //подключаем реакции на сигналы от раунда
    connect(round, &Round::roundStarted, this, &GameWidget::WhenRoundStarted);
    connect(round, &Round::firstPlayerAction, this, [this](quint64 action) { SetActionToLabel(p1Action, action, 80); });
    connect(round, &Round::secondPlayerAction, this, [this](quint64 action) { SetActionToLabel(p2Action, action, 80); });
    connect(round, &Round::roundFinished, this, &GameWidget::WhenRoundFinished);
}

//подключение контроллера
void GameWidget::SetController(TcpController* tcpController)
{
    controller = tcpController;
    controller->setParent(this);

    //подключаем реакции на сигналы контроллера и кнопки слотам контроллера
    connect(controller, &TcpController::nameReceived, this, &GameWidget::SetP2Name);
    connect(newRoundButton, &QPushButton::clicked, controller, &TcpController::SetReady);
    connect(this, &GameWidget::actionChosen, controller, &TcpController::SetAction);
    connect(controller, &TcpController::disconnected, this, &GameWidget::WhenDisconnected);
}

void GameWidget::SetP1Name(const QString& name)
{
    p1Name->setText(name + ": ");
}

void GameWidget::SetP2Name(const QString& name)
{
    p2Name->setText(name + ": ");
}

//здесь просто блокируем кнопку, чтобы больше не нажималась
void GameWidget::HandleNewRoundButton()
{
    newRoundButton->setDown(true);
    newRoundButton->setEnabled(false);
}

//обработка окончания раунда
void GameWidget::WhenRoundFinished(quint64 p1Action, quint64 p2Action, quint32 winner)
{
    //показываем результат
    SetResultToLabel(roundResult, winner, 70);
    //обновляем счетчики побед
    if (winner == 1) p1Won->setText(QString::number(p1Won->text().toUInt() + 1));
    if (winner == 2) p2Won->setText(QString::number(p2Won->text().toUInt() + 1));

    //добавляем в историю
    RoundWidget* widget = new RoundWidget(
                roundNumber->text().toUInt(),
                p1Name->text().split(':')[0],
                p1Action,
                winner,
                p2Action,
                p2Name->text().split(':')[0]
            );
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(widget->sizeHint());
    history->addItem(item);
    history->setItemWidget(item, widget);
    history->scrollToBottom();

    //разблокируем кнопку нового раунда
    newRoundButton->setEnabled(true);
    newRoundButton->setDown(false);
}

//обрабатываем начала раунда
void GameWidget::WhenRoundStarted()
{
    //обновляем счетчик раундов
    roundNumber->setText(QString::number(roundNumber->text().toUInt() + 1));

    //обнуляем показ состояния раунда
    SetResultToLabel(roundResult, 3, 70);
    SetActionToLabel(p1Action, 0, 70);
    SetActionToLabel(p2Action, 0, 70);

    //разблокируем кнопки действий
    SetActionButtonsEnable();
}

//выбор действия
void GameWidget::SelectAction(QPushButton *button, quint64 action)
{
    button->setDown(true);
    //блокируем кнопки
    SetActionButtonsDisable();
    //показываем выбранное действие
    //сигнализируем
    emit actionChosen(action);
}

//при дисконнекте блокируем кнопки и вывобим надпись "Disconnected"
void GameWidget::WhenDisconnected()
{
    SetActionButtonsDisable();
    newRoundButton->setEnabled(false);
    roundResult->setStyleSheet("color: rgb(0, 0, 0); font-size: 24px;");
    roundResult->setText("Disconnected");
    p1Action->setText(" ");
    p2Action->setText(" ");
}

//здесь разблокируем кнопки действий
void GameWidget::SetActionButtonsEnable()
{
    rockButton->setEnabled(true);
    scissorsButton->setEnabled(true);
    paperButton->setEnabled(true);
    rockButton->setDown(false);
    scissorsButton->setDown(false);
    paperButton->setDown(false);
}

//здесь блокируем кнопки действий
void GameWidget::SetActionButtonsDisable()
{
    rockButton->setEnabled(false);
    scissorsButton->setEnabled(false);
    paperButton->setEnabled(false);
}

//этот метод вызывыется кнопкой Quite - просто отправляем сигнал, выходом занимается родительский объект
void GameWidget::Quite()
{
    emit quite();
}
