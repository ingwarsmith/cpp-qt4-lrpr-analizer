#include "well.h"

//QMap<int, QString> Well::wellsInProject = QMap<int, QString>();

Well::Well()
{
    //wellID = Well::emptyWellID;
    wellID = 0;

    negativeResultProbability = 0.3;

    wellName = QString();

    wellDescription = QString();
}

//void Well::addNewWellIntoTheWellsArray(QString newWellName, int newWellId)
//{
//    if (newWellId == Well::emptyWellID_start)
//    {
//        if (Well::wellsInProject.isEmpty())
//        {
//            Well::wellsInProject.insert(newWellId, newWellName);
//        }
//        else
//        {
//            QList<int> IDs = Well::wellsInProject.keys();
//            qSort(IDs);
//            int newIDvalue = IDs.first()-1;

//            Well::wellsInProject.insert(newIDvalue, newWellName);
//        }
//    }
//}

