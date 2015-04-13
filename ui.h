#ifndef UI_H
#define UI_H

#include <QtWidgets/QtWidgets>

#include "gamemechanics.h"

class UI : public QWidget
{
    Q_OBJECT

public:
    UI(QWidget *parent = 0);
    ~UI();

private:
    GameMechanics *gameMechanics;
    QPushButton *button;
    QVector <QRadioButton*> rbVector;
    QVector <QLabel*> lblVector;
    QVector <QSpinBox*> sbVector;
    QVector <QHBoxLayout*> hblVector;
    QButtonGroup *distanceGroup, *elementGroup;
    QGroupBox *defenceGB, *damageGB;
    QScrollArea *scrollArea;
    QVBoxLayout *dataLayout, *damageLayout, *defenceLayout;
    QHBoxLayout *mainLayout;

private slots:
    void setsbMapSuffix(bool);
    void setSBData();
    void sendSBData();
    void sendRBData();
    void newWave();
};

#endif // UI_H
