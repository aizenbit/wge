#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();
}

UI::~UI()
{
    delete gameMechanics;
}

void UI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.begin(this);

    painter.setPen(QColor(255, 255, 255));

    auto mapSize = gameMechanics->getMapSize();
    auto cellWidth = this->width() / mapSize;
    auto cellHeight = this->height() / mapSize;

    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
        {
            switch (gameMechanics->getCell(i, j))
            {
            case empty:
                painter.setBrush(QColor(152, 251, 152));
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 (i + 1) * cellWidth, (j + 1) * cellHeight);
                break;

            case wall:
                painter.setBrush(QColor(139, 16, 19));
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 (i + 1) * cellWidth, (j + 1) * cellHeight);
                break;
            case busyByPlayer:
                painter.setBrush(QColor(0, 206, 209));
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 (i + 1) * cellWidth, (j + 1) * cellHeight);
                break;
            case busyByEnemy:
                painter.setBrush(QColor(255, 140, 0));
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 (i + 1) * cellWidth, (j + 1) * cellHeight);
                break;
            default:
                painter.setBrush(QColor(255, 255, 255));
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 (i + 1) * cellWidth, (j + 1) * cellHeight);
            };
        }

    painter.end();
}
