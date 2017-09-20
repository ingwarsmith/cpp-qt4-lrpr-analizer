#ifndef STRATUM_H
#define STRATUM_H

#include <QMap>
#include <QList>
#include <QObject>

class Stratum
{
public:
    Stratum();
    Stratum *retStratumPointer();
    QString nameOfStratum;
    int     numberOfStratumInCommonMapOfProject;
    QString description;

    //static QMap<int, QString> stratumNamesByNumber; - это будет элемент проекта
};

#endif // STRATUM_H
