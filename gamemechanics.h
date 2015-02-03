#ifndef GAMEMECHANICS_H
#define GAMEMECHANICS_H

#include <QObject>

class GameMechanics : public QObject
{
    Q_OBJECT
public:
    explicit GameMechanics(QObject *parent = 0);
    ~GameMechanics();

signals:

public slots:
};

#endif // GAMEMECHANICS_H
