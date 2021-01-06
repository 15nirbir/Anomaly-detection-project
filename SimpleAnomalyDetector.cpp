
#include "SimpleAnomalyDetector.h"



SimpleAnomalyDetector::SimpleAnomalyDetector(float threshold) {
	// TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    //save vector with the parameters name.
    std::vector<std::string> parametersName =ts.readParameters();
    int size=parametersName.size();
    //set shortcut to the map.
    std::map<std::string , std::vector<float>> param=ts.getParameters();
    //k run on the other parameters that didnt checked yet.
    int k=1;
    int temp=k;
    //loop on each parameter.
    for(std::string str:parametersName){
        k=temp;
        float maxCor =0;
        float* maxFirstCol;
        float* maxSecondCol;
        std::string maxCorParam;
        //each parameter compare with the next parameter to himself.
        for(int j=k;j<size;j++){
            int sizeOfColumn = ts.getColumnSize(str);
            float* firstColumn = &param[str][0];
            float* secondColumn = &param[parametersName[k]][0];
            // p is the correlation between 2 params in th csv.
            float p = pearson(firstColumn,secondColumn,sizeOfColumn);
            //check correlation among 2 parameters if they correlative
            // then append struct to cf vector after init struct.
            if(p > maxCor){
                maxCor = p;
                maxFirstCol = firstColumn;
                maxSecondCol = secondColumn;
                maxCorParam = parametersName[j];
            }
            k++;
        }
        if(maxCor > this->threshold){
            addCorrelatedFeatures(str ,maxCorParam , ts , maxCor , maxFirstCol , maxSecondCol);
        }
        temp++;
    }

}

//vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
//    std::vector<std::string> nameOfParameters = ts.readParameters();
//    //set shortcut to the map.
//    std::map<std::string , std::vector<float>> param=ts.getParameters();
//    std::vector<AnomalyReport> anomalyReport;
//    for (int i = 0; i < ts.getColumnSize(nameOfParameters[0]); ++i) {
//
//        for (correlatedFeatures correlatedFeatures:cf) {
//            Point point(param[correlatedFeatures.feature1][i] , param[correlatedFeatures.feature2][i]);
//            if(correlatedFeatures.threshold <= dev(point,correlatedFeatures.lin_reg)){
//                anomalyReport.push_back(AnomalyReport
//                (correlatedFeatures.feature1 + "-"+ correlatedFeatures.feature2 ,i+1 ));
//            }
//        }
//    }
//    return anomalyReport;
//}

Line SimpleAnomalyDetector::makeLineReg(float *x, float *y , size_t size) {
    Point* pPoint[size];

    for(int i=0;i < size;i++){
        Point* point =new Point(x[i],y[i]);
        pPoint[i]=point;

    }
    Line line = linear_reg(pPoint,size);
    for(int i=0;i<size;i++){
       delete pPoint[i];
    }
    return line;
}
float SimpleAnomalyDetector:: getThreshold(Line& lineReg ,float* x, float* y,size_t size){
    float max=0;
    for(int i=0;i<size;i++){
        Point point(x[i],y[i]);
        if(dev(point,lineReg)>max){
            max =(float) dev(point,lineReg);
        }
    }
    return max*1.2;
}
// add a correlative feature to vector of correlative feature's.
void SimpleAnomalyDetector::addCorrelatedFeatures(string &name1, string &name2, const TimeSeries &timeSeries,
                                                  float correlation, float *firstColumn, float *secondColumn) {
    if(correlation > this->threshold) {
        size_t length = timeSeries.getColumnSize(name1);
        struct correlatedFeatures correlatedFeatures;
        correlatedFeatures.feature1 = name1;
        correlatedFeatures.feature2 = name2;
        correlatedFeatures.corrlation = correlation;
        correlatedFeatures.lin_reg = makeLineReg(firstColumn, secondColumn, length);
        correlatedFeatures.threshold = getThreshold(correlatedFeatures.lin_reg, firstColumn, secondColumn, length);
        cf.push_back(correlatedFeatures);
    }
}

void SimpleAnomalyDetector::setTreshold(float newTreshold) {
    this->threshold = newTreshold;
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    std::vector<std::string> nameOfParameters = ts.readParameters();
    //set shortcut to the map.
    std::map<std::string , std::vector<float>> param=ts.getParameters();
    std::vector<AnomalyReport> anomalyReport;
    for (correlatedFeatures correlatedFeatures:cf){

            for (int i = 0; i < ts.getColumnSize(nameOfParameters[0]); ++i) {
            Point point(param[correlatedFeatures.feature1][i] , param[correlatedFeatures.feature2][i]);
            if(correlatedFeatures.threshold <= dev(point,correlatedFeatures.lin_reg)){
                anomalyReport.push_back(AnomalyReport
                                                (correlatedFeatures.feature1 + "-"+ correlatedFeatures.feature2 ,i+1 ));
            }
        }
    }
    return anomalyReport;
}

