#include "ui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UI ui;
    ui.show();

    return a.exec();
}
