#ifndef CALCULATIONTHREAD_H
#define CALCULATIONTHREAD_H

#include <QThread>

class CalculationThread : public QThread
{
    Q_OBJECT

public:
//    CalculationThread();
    void run();

    void retreiveDataForCalculation();

signals:
    void progressed(int percents);
};

#endif // CALCULATIONTHREAD_H
