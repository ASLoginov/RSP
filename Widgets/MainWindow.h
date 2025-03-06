#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BrowserWidget.h"
#include "GameWidget.h"

//этот класс по сути является диспетчером основных виджетов - создает, показывает, скрывает, удаляет
class MainWindow : public QWidget
{
    Q_OBJECT
    QString name = "Player"; //хранит имя игрока, которое передается другим виджетам при необходимости

    BrowserWidget* browser = nullptr; //поиск или создание игры
    GameWidget* game = nullptr; //собственно игра
public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void StarGame(QTcpSocket* socket); //метод для запуска игры, когда соединение установлено
    void Restart(); //закрытие игры и перезапуск браузера
};
#endif // MAINWINDOW_H
