#include "structure.h"

Structure::Structure(QObject *parent, bool notNew) : QObject(parent)
{
    Q_UNUSED(notNew);

    nameStructure = trUtf8("--Новая структура--");
//    if (!notNew)
//        trapsOnSructure << new Trap;
}

void Structure::setNameStruct(QString name)
{
    nameStructure = name;
}

QString Structure::getNameStruct()
{
    return nameStructure;
}

void Structure::setDescription(QString dsc)
{
    description = dsc;
}

QString Structure::getDescription()
{
    return description;
}
