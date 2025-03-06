#ifndef IENCRYPTIONMACHINE_H
#define IENCRYPTIONMACHINE_H

#include <QtGlobal>

//интерфейс для класса, инкапсулирующего шифрование ответа перед отправкой и расшифровку полученного
class IEncryptionMachine
{

public:
    virtual quint64 GetCode(quint64 num) = 0; //возвращает зашифрованный ответ num
    virtual QByteArray GetKey() = 0; //возвращает массив, содержащий ключ, необходимый для чтения шифра, полученного методом GetCode()
    virtual quint64 GetNumber(quint64 code, const QByteArray& keyArr) = 0; //возвращает результат расшифровки code, используя ключ, содержащийся в keyArr
    virtual void Refresh() = 0; //обновляет ключи шифрования
    virtual ~IEncryptionMachine() {}
};


#endif // IENCRYPTIONMACHINE_H
