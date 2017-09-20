#include "allocatetype.h"

void AllocateType::AllocateTypesMap_Init()
{
    if (!AllocateType::AllocateTypesMap.isEmpty()) return;

    AllocateType::AllocateTypesMap.insert(0, QObject::trUtf8("Двухтреугольное"));
    AllocateType::AllocateTypesMap.insert(1, QObject::trUtf8("Треугольное"));
    AllocateType::AllocateTypesMap.insert(2, QObject::trUtf8("Нормальное"));
    AllocateType::AllocateTypesMap.insert(3, QObject::trUtf8("Постоянное"));
    AllocateType::AllocateTypesMap.insert(4, QObject::trUtf8("Равномерное"));
}

QMap<int, QString> AllocateType::AllocateTypesMap = QMap<int, QString>();

AllocateType::AllocateType(QObject *parent) :
    QObject(parent)
{
    CurrentAllocType = 0;
    AllocateTypesMap_Init();
}

int AllocateType::getAllocTypeInt_Key()
{
    return CurrentAllocType;
}

QString AllocateType::getAllocTypeString_Val()
{
    return AllocateType::AllocateTypesMap.value(CurrentAllocType);
}

void AllocateType::setAllocTypeInt_Key(int newKey)
{
    CurrentAllocType = newKey;
}

QString AllocateType::getAllocTypeStringByKey(int neededKey)
{
    return AllocateType::AllocateTypesMap.value(neededKey);
}

QMap<int, QString> AllocateType::getMapOfAllocateTypes()
{
    return AllocateType::AllocateTypesMap;
}
