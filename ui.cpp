#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();
}

//------------------------------------------------------------

UI::~UI()
{
    delete gameMechanics;
}

//------------------------------------------------------------

void UI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.begin(this);

    auto mapSize = gameMechanics->getMapSize(); //using auto just for using auto
    float cellWidth = this->width() / float(mapSize);
    float cellHeight = this->height() / float(mapSize);

    for (int i = 0; i < mapSize; i++) //paint cells according to cell types
        for (int j = 0; j < mapSize; j++)
        {
                painter.setBrush(cellColor[gameMechanics->getCell(i, j)]);
                painter.setPen(cellColor[gameMechanics->getCell(i, j)]);
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 (i + 1) * cellWidth, (j + 1) * cellHeight);
        }

    painter.end();
}
