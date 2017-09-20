#ifndef TRAPDATAFORRESOURSESESTIMATION_H
#define TRAPDATAFORRESOURSESESTIMATION_H

#include <QString>
#include <QObject>
#include <QVector>

#include "allocatetype.h"
//#include "stratum.h"

struct MinAvgMax
{
    double minimum;
    double average;
    double maximum;
};

class TrapDataForResoursesEstimation
{
public:
    TrapDataForResoursesEstimation();

    int       numberInTrapsMap;

    MinAvgMax Area;                 // S
    MinAvgMax EffectiveThick;       // hэф
    MinAvgMax PorosityCoeff;        // Кп, доли единицы
    MinAvgMax OilSaturationCoeff;   // Кн, доли единицы
    MinAvgMax DensityOfOil;         // gamma, менее 1 г/см3
    MinAvgMax ShrinkingCoeff;       // b (усадка), доли единицы
    MinAvgMax KIN;                  // доли единицы

    double    determinedKIN;        // доли единицы

//    AllocateType *allocationType;    // тип распределения для данной ловушки

    int       numberOfAllocationType; // номер типа распределения для данной ловушки

//    QString   commentForTrap;

    int       numberOfWells;

    double    probabilityOfDepositExisting; // вероятность существования залежи в ловушке

    // Будет перенесено в класс Trap
//    QVector<int> numbersOfWellsRecommendedForExplorationOfTrap;

    // Будет перенесено в класс проекта
//    void setSelectedStratumName(int numberOfStratum);
//    void setNewStratumNameAndAddItIntoArrayOfStratums(QString newName);
};

#endif // TRAPDATAFORRESOURSESESTIMATION_H
