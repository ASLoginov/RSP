#ifndef UTILITY_H
#define UTILITY_H

#include <QLabel>
#include <QIcon>

//полезные функции для управления QLabel в контексте игры
void SetActionToLabel(QLabel* label, quint64 action, qint32 size); //установить в QLabel отображение действия
void SetResultToLabel(QLabel* label, quint32 result, qint32 size); //установить в QLabel отображение результата

#endif // UTILITY_H
