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
    QVector <QSpinBox*> sbVector;
    QMap <QString, QHBoxLayout*> hblMap;
    QButtonGroup *distanceGroup, *elementGroup;
    QScrollArea *scrollArea;
    QVBoxLayout *dataLayout;
    QHBoxLayout *mainLayout;

    QVector <QString> strVector;

private slots:
    void setsbMapSuffix(bool);
    void setSBData();
    void sendSBData();
    void sendRBData();

public:
    UI(QWidget *parent = 0);
    ~UI();

};

#endif // UI_H
