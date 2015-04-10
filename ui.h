#ifndef UI_H
#define UI_H

#include <QtWidgets/QtWidgets>

#include "gamemechanics.h"

class UI : public QWidget
{
    Q_OBJECT

private:
    GameMechanics *gameMechanics;
    QPushButton *button;
    QMap <QString, QRadioButton*> rbMap;
    QMap <QString, QLabel*> lblMap;
    QMap <QString, QSpinBox*> sbMap;
    QMap <QString, QHBoxLayout*> hblMap;
    QButtonGroup *distanceGroup, *elementGroup;
    QScrollArea *scrollArea;
    QVBoxLayout *dataLayout;
    QHBoxLayout *mainLayout;

public:
    UI(QWidget *parent = 0);
    ~UI();

};

#endif // UI_H
