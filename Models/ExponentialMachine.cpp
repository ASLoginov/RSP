#include "ExponentialMachine.h"
#include <QRandomGenerator>
#include <QDataStream>
#include <QIODevice>

//расширенный алгоритм Евклида, нужен для получения мультипликативно обратного по модулю
quint64 ExtendedEuclideanAlgorithm(quint64 a, quint64 m)
{
    quint64 q, r, x, x1, x2;

    x2 = 1;
    x1 = 0;

    while (m > 0)
    {
        q = a / m;
        r = a - q * m;
        a = m;
        m = r;

        x = x2 - q * x1;
        x2 = x1;
        x1 = x;
    }
    return x2;
}

//по имеющемуся ключу для шифрования и модулю создаем ключ для расшифровки
qint64 makeKey(qint64 a, qint64 m)
{
    qint64 x = ExtendedEuclideanAlgorithm(a, m);
    x = (x % m + m) % m;

    return x;
}

//сумма по модулю
quint64 sumMod(quint64 x, quint64 y, quint64 m)
{
    if(m - x > y) return  x + y;
    return  y - (m - x);
}

//произведение по модулю
quint64 mulMod(quint64 x, quint64 y, quint64 m)
{
    quint64 tmp;
    quint64 res = 0 ;

    if(x > y)
    {
      tmp = x ;
      x = y;
      y = tmp;
    }

    while(x)
    {
      if(x & 1) res = sumMod(res, y, m);

      y  = sumMod(y , y , m);
      x >>= 1;
    }

    return  res ;
}

//проверка числа на простоту
bool isPrime(quint64 a)
{
    quint64 x;
    quint64 n = sqrt(a)/6 + 1;

    if(a == 2) return true;
    if(a == 3) return true;

    if(!(a % 2)) return false;
    if(!(a % 3)) return false;

    for(quint64 i = 1; i < n; i++)
    {
        x = 6 * i;
        if(!(a % (x-1))) return false;
        if(!(a % (x+1))) return false;
    }

    return true;
}

//генерируем простое число из заданного диапазона
quint64 makePrimeDigit()
{
    quint32 a = 0x80000000;
    quint32 b = 0xfffffff0;
    quint64 x = QRandomGenerator::global()->bounded(a, b);

    if(!(x % 2)) x++;

    while(!isPrime(x))
    {
        x += 2;
    }

    return x;
}

//обновляем ключи шифрования
void ExponentialMachine::Refresh()
{
    quint64 easy1, easy2;
    module = 0xF000000000000000;

    //генерируем два простых числа, чтобы модуль, равный их произведению, был не очень большой
    while(module > 0x7FFFFFFFFFFFFFFF)
    {
        easy1 = makePrimeDigit();
        easy2 = makePrimeDigit();
        module = easy1 * easy2;
    }

    encryptKey = 65537; //ключ для шифрования берем стандартный, обеспечивающий быстрое шифрование
    quint64 m = (easy1 - 1) * (easy2 - 1); //функция Эйлера

    //получаем ключ для расшифровки
    decryptKey = makeKey(encryptKey, m);
}

//функция для шифрования и дешифрования
//если передать число, ключ шифрования и модуль, то вернет шифр
//если передать шифр, ключ дешифрования и модуль, то вернет изначальное число
quint64 Encrypt(quint64 num, quint64 key, quint64 mod)
{
    quint64 res = 1 ;

    while(key)
    {
        if(key & 1) res = mulMod(res, num, mod);

        num = mulMod(num, num, mod);
        key >>= 1;
    }

    return res;
}

//шифруем
quint64 ExponentialMachine::GetCode(quint64 num)
{
    return Encrypt(num + encryptKey, encryptKey, module);
}

//упаковываем ключ дешифровки и модуль в QByteArray
QByteArray ExponentialMachine::GetKey()
{
    QByteArray res;
    QDataStream stream(&res, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_8);
    stream << decryptKey << module;
    return res;
}

//достаем ключ и модуль из массива и дешифруем
quint64 ExponentialMachine::GetNumber(quint64 code, const QByteArray& keyArr)
{
    QDataStream stream(keyArr);
    stream.setVersion(QDataStream::Qt_6_8);
    quint64 key, mod;
    stream >> key >> mod;
    return Encrypt(code, key, mod) - encryptKey;
}


ExponentialMachine::ExponentialMachine()
{

}
