#ifndef DNA_H
#define DNA_H

#include <QObject>
#include <vector>

class DNA : public QObject
{
    Q_OBJECT

public:
    enum GenType {
        HP,
        defence,
        genTypeCount
    };

    struct Gen {
        unsigned int value;
        unsigned int maxValue;
    };

private:
    std::vector<Gen> dna;


public:
    explicit DNA(QObject *parent = 0);
    ~DNA();


signals:

public slots:
};

#endif // DNA_H
