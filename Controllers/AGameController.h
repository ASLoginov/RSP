#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
//абстрактный контроллер для подключения работы с классом Round
//предоставляем интерфейс взаимодействия раунда с внешним миром (виджетом и оппонентом) через сигнально-слотную архитектуру
class AGameController : public QObject
{
    Q_OBJECT
public:
    AGameController(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    virtual void SendReady() = 0; //отправить сигнал о готовности к новому раунду
    virtual void SendCode(quint64 code) = 0; //отправить зашифрованное действие
    virtual void SendKey(const QByteArray& keyArr) = 0; //отправить ключ для чтения шифра
    virtual void SetAction(quint64 action) = 0; //установить выбранное действие
    virtual void SetReady() = 0; //установить состояние готовности к новому раунду

signals:
    void readyReceived(); //получен сигнал о готовности оппонента
    void readySet(); //сигнал о входе в состояние готовности
    void actionSet(quint64 action); //сигнал о выбранном действии
    void codeReceived(quint64 code); //получен шифр с действием оппонента
    void keyReceived(const QByteArray& keyArr); //получен ключ к шифру оппонента
};

#endif // GAMECONTROLLER_H
