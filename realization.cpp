#include "realization.h"

#include <omp.h>

Realization::Realization(QObject *parent) : QObject(parent)
{
    timeOfStarting = QDateTime::currentDateTime();

    name = timeOfStarting.date().toString(Qt::ISODate) + QString(", ") + timeOfStarting.time().toString();
}

void Realization::retreiveData(QMap<int, Structure *> structures,
                               QMap<int, Trap *> traps,
                               QMap<int, Well *> wells,
                               QMap<int, Stratum *> stratums)
{
    structuresInCalculation = QMap<int, Structure *>(structures);
    trapsInCalculation = QMap<int, Trap *>(traps);
    wellsInCalculation = QMap<int, Well *>(wells);
    stratumsInCalculation = QMap<int, Stratum *>(stratums);
}

