#ifndef UI_H
#define UI_H

#include <QWidget>

#include "gamemechanics.h"

class UI : public QWidget
{
    Q_OBJECT

private:
    GameMechanics *gameMechanics;
public:
    UI(QWidget *parent = 0);
    ~UI();
    virtual void paintEvent(QPaintEvent *);
};

#endif // UI_H
