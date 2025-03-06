#include "Widgets/MainWindow.h"

#include <QApplication>

//в мэйне просто запускаем приложение
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
