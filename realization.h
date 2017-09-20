#ifndef REALIZATION_H
#define REALIZATION_H

#include <QObject>
#include <QDateTime>
#include <QVector>

#include "structure.h"
#include "well.h"

#include "calculationthread.h"

class Realization : public QObject
{
    Q_OBJECT
public:
    explicit Realization(QObject *parent = 0);
    QDateTime   timeOfStarting;
    QString     name;
    void        retreiveData(QMap<int, Structure* >  structures,
                             QMap<int, Trap* >       traps,
                             QMap<int, Well* >       wells,
                             QMap<int, Stratum* >    stratums);

    struct morestProbabileResourcesOrReservesValueInfo
    {
        double  morestProbibleGeologicalRes;
        double  morestProbibleExtractableRes;
        double  probabilityOfTheseEstimation;
    };

    struct VectorsOfData
    {
        QVector<double> Vector_Resources[2]; // 0 - геологические, 1 - извлекаемые ресурсы
        QVector<int>    Vector_TrapID;
        QVector<double> Vector_Areas;
        QVector<double> Vector_EffectiveThicks;
        QVector<double> Vector_PorosityCoeff;
        QVector<double> Vector_OilSaturationCoeff;
        QVector<double> Vector_DensityOfOil;
        QVector<double> Vector_ShrinkingCoeff;
        int             Integral;
        int             Techno;
        double          summaGeolResources;
        double          summaExtrResources;
        morestProbabileResourcesOrReservesValueInfo mPRVinfo;
    };

private:
    QMap<int, Structure* >  structuresInCalculation;
    QMap<int, Trap* >       trapsInCalculation;
    QMap<int, Well* >       wellsInCalculation;
    QMap<int, Stratum* >    stratumsInCalculation;

signals:

public slots:
};

#endif // REALIZATION_H
