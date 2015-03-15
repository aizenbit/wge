#ifndef UI_H
#define UI_H

#include <QtWidgets/QtWidgets>

#include "gamemechanics.h"

class UI : public QWidget
{
    Q_OBJECT

private:
    GameMechanics *gameMechanics;
    QPushButton *stepPB;
    QVBoxLayout *mainLayout;

public:
    UI(QWidget *parent = 0);
    ~UI();

};

#endif // UI_H
