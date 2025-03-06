#include "Round.h"

//создаем раунд, использующий переданное устройство шифрования
Round::Round(IEncryptionMachine *machine, QObject *parent) : QObject(parent)
{
    encryptionMachine = machine;
}

void Round::SetController(AGameController *controller)
{
    //устанавливаем управление раундом через контроллер
    connect(controller, &AGameController::readyReceived, this, &Round::ReadyReceived);
    connect(controller, &AGameController::readySet, this, &Round::GetReady);
    connect(controller, &AGameController::actionSet, this, &Round::SetFirstPlayerAction);
    connect(controller, &AGameController::codeReceived, this, &Round::SetSecondPlayerCode);
    connect(controller, &AGameController::keyReceived, this, &Round::UseKey);

    //устанавливаем взаимодействие со вторым игроком через контроллер
    connect(this, &Round::ready, controller, &AGameController::SendReady);
    connect(this, &Round::firstPlayerCode, controller, &AGameController::SendCode);
    connect(this, &Round::firstPlayerKey, controller, &AGameController::SendKey);
}

//определяем победителя и завершаем раунд
void Round::DetermineWinner()
{
    quint32 winner = 0; //ничья
    if (p1Action % 3 == p2Action - 1) winner = 1; //победил первый игрок
    else if (p2Action % 3 == p1Action - 1) winner = 2; //победил второй игрок

    //отправляем сигнал с результатами раунда
    emit roundFinished(p1Action, p2Action, winner);
}

Round::~Round()
{
    delete encryptionMachine; //предполагается, что шифрующее устройство нигде больше не используется, поэтому удаляем
}


//сохраняем ответ первого игрока
void Round::SetFirstPlayerAction(quint64 action)
{
    p1Action = action;
    emit firstPlayerAction(p1Action);

    //отправляем зашифрованное действие
    emit firstPlayerCode(encryptionMachine->GetCode(p1Action));

    //если шифр второго игрока уже полученн, отправляем ключ
    if (p2Code && p1Action)
        emit firstPlayerKey(encryptionMachine->GetKey());
}

//сохраняем действие второго игрока
void Round::SetSecondPlayerCode(quint64 code)
{
    p2Code = code;

    //если действие первого игрока уже выбрано, отправляем ключ
    if (p1Action && p2Code)
        emit firstPlayerKey(encryptionMachine->GetKey());
}

//получаем ключ
void Round::UseKey(const QByteArray& keyArr)
{
    //расшифровываем действие вторго игрока
    p2Action = encryptionMachine->GetNumber(p2Code, keyArr);
    emit secondPlayerAction(p2Action);

    //определяем победителя
    DetermineWinner();
}

//объявляем новый раунд
void Round::NewRound()
{
    p1Ready = false;
    p2Ready = false;
    emit roundStarted();
}

//обнуляем все данные раунда и обновляем ключи шифрования, и объявляем готовность к новому раунду
void Round::GetReady()
{
    p1Action = 0;
    p2Action = 0;
    p2Code = 0;
    encryptionMachine->Refresh();
    p1Ready = true;
    emit ready();
    //если все готовы, объяляем новый раунд
    if (p1Ready && p2Ready) NewRound();
}

//проставляем флаг готовности второго игрока
void Round::ReadyReceived()
{
    p2Ready = true;
    //если все готовы, объяляем новый паунд
    if (p1Ready && p2Ready) NewRound();
}
