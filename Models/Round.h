#ifndef ROUND_H
#define ROUND_H

#include <QObject>
#include "ExponentialMachine.h"
#include "Controllers/AGameController.h"

//модель одного раунда игры, инкапсулирует логику прохождения всех необхдимых для выявления победителя фаз
//FirstPlayer - это пользователь данного экземпляра приложения, SecondPlayer - его оппонент
class Round : public QObject
{
    Q_OBJECT

    quint64 p1Action = 0; //хранить выбранный ответ первого игрока
    quint64 p2Action = 0; //хранит расшифрованный ответ второго игрока
    quint64 p2Code = 0; //хранит зашифрованный ответ второго игрока
    bool p1Ready = false; //флаг готовности первого игрока
    bool p2Ready = false; //флаг готовности второго игрока

    IEncryptionMachine* encryptionMachine; //объект, отвечающий за шифрование
    AGameController* controller;

    void DetermineWinner(); //определяет победителя и завершает раунд

public:
    explicit Round(IEncryptionMachine* machine, QObject *parent = nullptr); //в конструктор передается объект для шифрования
    void SetController(AGameController* controller); //присоединение контроллера
    ~Round();

public slots:
    void SetFirstPlayerAction(quint64 ans); //записывает ответ первого игрока
    void SetSecondPlayerCode(quint64 code); //записывает шифрованный ответ второго игрока
    void UseKey(const QByteArray& keyArr); //получает ключ для чтения ответа второго игрока
    void NewRound(); //запускает новый раунд
    void GetReady(); //готовит новый раунд
    void ReadyReceived(); //получает сигнал о готовности второго игрока

signals:
    void ready(); //сигнал готовности к новому раунду
    void roundStarted(); //сигнал начала нового раунда
    void firstPlayerAction(quint64 action); //действие первого инрока
    void firstPlayerCode (quint64 code); //зашифрованное действие первого игрока
    void firstPlayerKey(const QByteArray& keyArr); //ключ к шифру первого игрока
    void roundFinished(quint64 p1Action, quint64 p2Action, quint32 winner); //сигнал об окончании раунда, содержащий результаты раунда
    void secondPlayerAction(quint64 action); //расшифрованное действие второго игрока
};

#endif // ROUND_H
