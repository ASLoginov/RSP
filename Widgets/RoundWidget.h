#ifndef ROUNDWIDGET_H
#define ROUNDWIDGET_H

#include <QWidget>

//это класс для отображения сыгранного раунда в истории раундов
//имеет чисто визуальное назначение
class RoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoundWidget(quint32 roundNumber,
                         const QString& p1Name,
                         const quint64 p1Action,
                         const quint32 winner,
                         const quint64 p2Action,
                         const QString& p2Name,
                         QWidget* parent = nullptr);

};

#endif // ROUNDWIDGET_H
