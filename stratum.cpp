#include "stratum.h"

//QMap<int, QString> Stratum::stratumNamesByNumber = QMap<int, QString>();

Stratum::Stratum()
{
    nameOfStratum = QObject::trUtf8("--- новый пласт ---");
    numberOfStratumInCommonMapOfProject = -1;
}

Stratum *Stratum::retStratumPointer()
{
    return this;
}

