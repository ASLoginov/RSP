#ifndef ENCRYPTIONMACHINE_H
#define ENCRYPTIONMACHINE_H

#include "IEncryptionMachine.h"

//класс, реализующий интерфейс IEncryptionMachine методом экспоненциального шифрования
class ExponentialMachine : public IEncryptionMachine
{

    quint64 encryptKey; //часть ключа для шифрования ответа перед отправкой
    quint64 decryptKey; //часть ключа для расшифровки переданного ответа
    quint64 module; //общая часть для обоих ключей

public:
    explicit ExponentialMachine();
    quint64 GetCode(quint64 num); //возвращает зашифрованный ответ num
    QByteArray GetKey(); //возвращает массив, содержащий обе части, необходимые для чтения шифра, полученного методом GetCode()
    quint64 GetNumber(quint64 code, const QByteArray& keyArr); //возвращает результат расшифровки code, используя ключ, содержащийся в keyArr
    void Refresh(); //обновляет ключи шифрования
};

#endif // ENCRYPTIONMACHINE_H
