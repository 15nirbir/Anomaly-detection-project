

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "minCircle.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "timeseries.h"

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	float centerX;
	float  centerY;
	float radius;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{

protected:
    Line makeLineReg(float* x , float* y , size_t size);
    float getThreshold(Line& lineReg ,float* x, float* y,size_t size);
    void addCorrelatedFeatures(std:: string& name1 , std:: string& name2 ,const TimeSeries& timeSeries
            , float correlation , float* firstColumn , float* secondColumn);
    vector<correlatedFeatures> cf;
    float threshold;
public:
	SimpleAnomalyDetector(float threshold = 0.9);
	virtual ~SimpleAnomalyDetector();
    void setTreshold(float newTreshold);
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}




};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
