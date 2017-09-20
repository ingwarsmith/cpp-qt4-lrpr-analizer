#ifndef WELL_H
#define WELL_H

#include <QMap>
#include <QList>
#include <QObject>

class Well
{
public:
    Well();

    QString     wellName;
    int         wellID;
    QString     wellDescription;

    double      negativeResultProbability;

    static const int emptyWellID_start = -777;
//    static QMap<int, QString> wellsInProject; - будет перенесено на уровень проекта

//    void addNewWellIntoTheWellsArray(QString newWellName, int newWellId = emptyWellID_start );
};

#endif // WELL_H
