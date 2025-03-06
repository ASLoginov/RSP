#include "Utility.h"

//устанавливаем в QLabel иконку переданного действия выбранного размера
void SetActionToLabel(QLabel* label, quint64 action, qint32 size)
{
    switch (action)
    {
    case 0:
        label->setText("?");
        label->setStyleSheet("font-size: " + QString::number(size) + "px;");
        break;
    case 1:
        label->setPixmap(QIcon(":/ico/ico/rock.png").pixmap(size, size));
        break;
    case 2:
        label->setPixmap(QIcon(":/ico/ico/scissors.png").pixmap(size, size));
        break;
    case 3:
        label->setPixmap(QIcon(":/ico/ico/paper.png").pixmap(size, size));
        break;
    default:
        label->setText("error");
        break;
    }
}


//устанавливаем в QLabel текст результата определенного цвета и выбранного размера
void SetResultToLabel(QLabel *resultLabel, quint32 result, qint32 size)
{
    switch (result)
    {
    case 0:
        resultLabel->setText("Draw");
        resultLabel->setStyleSheet("color: rgb(0, 0, 142); font-size: " + QString::number(size) + "px;");
        break;
    case 1:
        resultLabel->setText("Won");
        resultLabel->setStyleSheet("color: rgb(0, 142, 0); font-size: " + QString::number(size) + "px;");
        break;
    case 2:
        resultLabel->setText("Lost");
        resultLabel->setStyleSheet("color: rgb(142, 0, 0); font-size: " + QString::number(size) + "px;");
        break;
    case 3:
        resultLabel->setText(" ");
        resultLabel->setStyleSheet("color: rgb(0, 0, 0); font-size: " + QString::number(size) + "px;");
        break;
    default:
        resultLabel->setText("error");
        resultLabel->setStyleSheet("color: rgb(0, 0, 0); font-size: " + QString::number(size) + "px;");
        break;
    }
}
