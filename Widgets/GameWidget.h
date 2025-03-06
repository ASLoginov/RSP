#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "Models/Round.h"
#include "Controllers/TcpController.h"

//виджет для отображения процесса игры и управления
class GameWidget : public QWidget
{
    Q_OBJECT

    QPushButton* quiteButton; //кнопка выхода из игры обратно в браузер

    QListWidget* history; //история сыгранных раундов
    QLabel* p2Name; //имя второго игрока, то есть оппонента
    QLabel* p2Won; //счетчик побед оппонета
    QLabel* p2Action; //отображение действия оппонента в текущем раунде когда оно известно
    QLabel* roundResult; //отображение результата раунда после окончания
    QLabel* p1Action; //отображение выбранного действия игрока
    QLabel* p1Name; //имя игрока
    QLabel* p1Won; //счетчик побед
    QLabel* roundNumber; //номер текущего раунда

    //три кнопки выбора действия
    QPushButton* rockButton;
    QPushButton* scissorsButton;
    QPushButton* paperButton;
    //кнопка готовности к новому раунду
    QPushButton* newRoundButton;

    Round* round; //объект, инкапсулирующий игровую механику
    TcpController* controller; //контроллер для управления игрой

    void SetActionButtonsEnable(); //разблокировать кнопки действий
    void SetActionButtonsDisable(); //заблокировать кнопки действий

public:
    explicit GameWidget(QWidget *parent = nullptr);
    void SetRound(Round* roundObject); //подключить раунд
    void SetController(TcpController* tcpController); //подключить контроллер
    void SetP1Name(const QString& name); //установить имя игрока
    void SetP2Name(const QString& name); //установить имя оппонента

public slots:
    void HandleNewRoundButton(); //обработка кнопки нового раунда
    void WhenRoundFinished(quint64 p1Action, quint64 p2Action, quint32 winner); //действия при окончании раунда
    void WhenRoundStarted(); //действия при старте раунда
    void SelectAction(QPushButton* button, quint64 action); //выбор действия
    void WhenDisconnected(); //действия при разрыве соединения
    void Quite(); //действия, чтобы выйти

signals:
    void actionChosen(quint64 action); //выбрано действие
    void quite(); //сигнализируем о намерении выйти
};

#endif // GAMEWIDGET_H
