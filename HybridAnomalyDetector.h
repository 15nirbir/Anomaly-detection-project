

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
protected:
    Point** toPoints(float* x , float* y , size_t size);
    void addCorrelatedFeatures(std:: string& name1 , std:: string& name2 ,const TimeSeries& timeSeries, float correlation , float* firstColumn , float* secondColumn,Circle minCircle);
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
