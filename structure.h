#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <QObject>
#include <QVector>

#include "trap.h"

class Structure : public QObject
{
    Q_OBJECT
public:
    explicit Structure(QObject *parent = 0, bool notNew = false);
    //QVector<Trap*>      trapsOnSructure;
    QVector<int>        trapsNumbersFromCommonMap_onStructure;
    void                setNameStruct(QString name);
    QString             getNameStruct();
    void                setDescription(QString dsc);
    QString             getDescription();    

private:
    QString nameStructure;
    QString description;

signals:
    void sigTrapWasCreated(Trap *);

public slots:
};

#endif // STRUCTURE_H
