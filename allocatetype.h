#ifndef ALLOCATETYPE_H
#define ALLOCATETYPE_H

#include <QObject>
#include <QMap>

class AllocateType : public QObject
{
    Q_OBJECT
public:
    explicit AllocateType(QObject *parent = 0);
    QString getAllocTypeString_Val();
    int getAllocTypeInt_Key();
    void setAllocTypeInt_Key(int newKey);
    QString getAllocTypeStringByKey(int neededKey);
    QMap<int, QString> getMapOfAllocateTypes();
    
signals:
    
public slots:

private:
    int CurrentAllocType;   // тип распределения текущего объекта класса
    static QMap<int, QString> AllocateTypesMap;
    static void AllocateTypesMap_Init();
};

#endif // ALLOCATETYPE_H
