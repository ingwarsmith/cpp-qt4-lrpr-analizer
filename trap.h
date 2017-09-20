#ifndef TRAP_H
#define TRAP_H

#include <QObject>

#include "trapdataforresoursesestimation.h"
#include "stratum.h"

class Trap : public QObject, public Stratum//: public TrapDataForResoursesEstimation, Stratum
{
    Q_OBJECT
public:
    Trap();
    ~Trap();

    TrapDataForResoursesEstimation *baseData;

    QString   nameTrap;             // имя ловушки (с указанием индекса пласта)

    AllocateType *allocationType;    // тип распределения для данной ловушки

    QString   commentForTrap;

    QVector<int> IDofWellsRecommendedForExplorationOfTrap;
};

#endif // TRAP_H
