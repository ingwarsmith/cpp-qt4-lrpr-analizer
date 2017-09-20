#include "trapdataforresoursesestimation.h"

TrapDataForResoursesEstimation::TrapDataForResoursesEstimation()
{
    Area.average = 0.0;
    Area.maximum = 0.0;
    Area.minimum = 0.0;

    EffectiveThick.average = 0.0;
    EffectiveThick.maximum = 0.0;
    EffectiveThick.minimum = 0.0;

    PorosityCoeff.average = 0.0;
    PorosityCoeff.maximum = 0.0;
    PorosityCoeff.minimum = 0.0;

    OilSaturationCoeff.average = 0.0;
    OilSaturationCoeff.maximum = 0.0;
    OilSaturationCoeff.minimum = 0.0;

    DensityOfOil.average = 0.0;
    DensityOfOil.maximum = 0.0;
    DensityOfOil.minimum = 0.0;

    ShrinkingCoeff.average = 0.0;
    ShrinkingCoeff.maximum = 0.0;
    ShrinkingCoeff.minimum = 0.0;

    KIN.average = 0.0;
    KIN.maximum = 0.0;
    KIN.minimum = 0.0;

    determinedKIN = 0.0;

    numberOfWells = 0;

    probabilityOfDepositExisting = 0.0;

    //allocationType = new AllocateType();

    //numbersOfWellsRecommendedForExplorationOfTrap = QVector<int>();
}
