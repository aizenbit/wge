#ifndef UI_H
#define UI_H

#include <QWidget>

#include "gamemechanics.h"

class UI : public QWidget
{
    Q_OBJECT

private:
    GameMechanics *gameMechanics;
    QColor cellColor[cellTypeCount] {QColor(0, 0, 0),
                                     QColor(152, 251, 152),
                                     QColor(253, 236, 143),
                                     QColor(220, 220, 220),
                                     QColor(0, 206, 209),
                                     QColor(255, 140, 0)};
public:
    UI(QWidget *parent = 0);
    ~UI();
    virtual void paintEvent(QPaintEvent *);
};

#endif // UI_H
