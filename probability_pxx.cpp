#include "probability_pxx.h"

Probability_Pxx::Probability_Pxx(int threshold)
{
    if (threshold == 0)
    {
        m_thresholdValueInProcents = 90;
    }
    else
    {
        m_thresholdValueInProcents = threshold;
    }

    m_MAX_ValResourcesInData = m_MIN_ValResourcesInData = m_Step = 0.0;

    NumbOfIntervals = 50;

    if (!valuesNumInTheIntervals.isEmpty())
        valuesNumInTheIntervals.clear();

    if (!accumulatedValuesNumByIntervalsForReversedJoke.isEmpty())
        accumulatedValuesNumByIntervalsForReversedJoke.clear();
}

void Probability_Pxx::setThreshold(int newThresholdVal)
{
    m_thresholdValueInProcents = newThresholdVal;
}

int Probability_Pxx::getThreshold()
{
    return m_thresholdValueInProcents;
}

int Probability_Pxx::getNumbOfIntervals()
{
    return NumbOfIntervals;
}

QVector<int> Probability_Pxx::getValuesNumInTheIntervals() const
{
    return valuesNumInTheIntervals;
}

QVector<int> Probability_Pxx::getAccumulatedValuesNumByIntervals() const
{
    return accumulatedValuesNumByIntervalsForReversedJoke;
}

