#ifndef PROBABILITY_PXX_H
#define PROBABILITY_PXX_H

#include <QVector>

class Probability_Pxx
{
public:
    Probability_Pxx(int threshold = 0);
    void setThreshold(int newThresholdVal);
    int getThreshold();
    int getNumbOfIntervals();
    QVector<int> getValuesNumInTheIntervals() const;
    QVector<int> getAccumulatedValuesNumByIntervals() const;

protected:
    int m_thresholdValueInProcents;
    double m_MAX_ValResourcesInData;
    double m_MIN_ValResourcesInData;
    double m_Step;
    int NumbOfIntervals;
    QVector<int> valuesNumInTheIntervals,           // число значений по интервалам
    accumulatedValuesNumByIntervalsForReversedJoke; // обратная кумулятива (reversed joke) вероятности (интегральное распределение)
};

#endif // PROBABILITY_PXX_H
