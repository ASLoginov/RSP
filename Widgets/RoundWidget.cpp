#include "RoundWidget.h"
#include "Utility/Utility.h"
#include <QBoxLayout>

RoundWidget::RoundWidget(quint32 roundNumber,
                         const QString &p1Name,
                         const quint64 p1Action,
                         const quint32 winner,
                         const quint64 p2Action,
                         const QString &p2Name,
                         QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);

    QLabel* roundLabel = new QLabel(QString::number(roundNumber));
    roundLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(roundLabel);

    QLabel* p1Label = new QLabel(p1Name);
    p1Label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(p1Label);

    QLabel* action1Label = new QLabel();
    action1Label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    SetActionToLabel(action1Label, p1Action, 24);
    layout->addWidget(action1Label);

    QLabel* resultLabel = new QLabel();
    resultLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    SetResultToLabel(resultLabel, winner, 14);
    layout->addWidget(resultLabel);

    QLabel* action2Label = new QLabel();
    action2Label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    SetActionToLabel(action2Label, p2Action, 24);
    layout->addWidget(action2Label);

    QLabel* p2Label = new QLabel(p2Name);
    p2Label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(p2Label);
}
