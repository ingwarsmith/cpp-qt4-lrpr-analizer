#include "trap.h"

Trap::Trap()
    : Stratum()
{
    baseData = new TrapDataForResoursesEstimation;

    allocationType = new AllocateType;

    nameTrap = trUtf8("-- Новая ловушка --");
}

Trap::~Trap()
{
    delete baseData;
    delete allocationType;
}

