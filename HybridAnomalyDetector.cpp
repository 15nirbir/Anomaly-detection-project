
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}
void HybridAnomalyDetector::learnNormal(const TimeSeries& ts)  {
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
        Point** points;
        //each parameter compare with the next parameter to himself.
        for(int j=k;j<size;j++){

                int sizeOfColumn = ts.getColumnSize(str);
                float* firstColumn = &param[str][0];
                float* secondColumn = &param[parametersName[k]][0];
                // p is the correlation between 2 params in th csv.
                float p = pearson(firstColumn,secondColumn,sizeOfColumn);
                Point** ps = toPoints(firstColumn , secondColumn , sizeOfColumn);

                if(p > maxCor){
                    maxCor = p;
                    maxFirstCol = firstColumn;
                    maxSecondCol = secondColumn;
                    maxCorParam = parametersName[j];
                    points = ps;
                }

                k++;
            }
        if(maxCor > 0.5){
            Circle circle = findMinCircle(points , ts.getColumnSize(str));
            addCorrelatedFeatures(str ,maxCorParam , ts , maxCor , maxFirstCol , maxSecondCol , circle);
        }
        temp++;
        }
}
Point** HybridAnomalyDetector::toPoints(float* x, float* y, size_t size) {
    Point** ps=new Point*[size];
    for(size_t i=0;i<size;i++){
        ps[i]=new Point(x[i],y[i]);
    }
    return ps;
}

void HybridAnomalyDetector::addCorrelatedFeatures(string &name1, string &name2, const TimeSeries &timeSeries,
                                                  float correlation, float *firstColumn, float *secondColumn , Circle minCircle) {
    SimpleAnomalyDetector::addCorrelatedFeatures(name1, name2, timeSeries, correlation
                                                 , firstColumn, secondColumn);
    if(correlation < threshold && correlation > 0.5){
        size_t length = timeSeries.getColumnSize(name1);
        struct correlatedFeatures correlatedFeatures;
        correlatedFeatures.feature1 = name1;
        correlatedFeatures.feature2 = name2;
        correlatedFeatures.corrlation = correlation;
        correlatedFeatures.centerX = minCircle.center.x;
        correlatedFeatures.centerY = minCircle.center.y;
        correlatedFeatures.radius = minCircle.radius*1.1;
        correlatedFeatures.threshold = correlatedFeatures.radius;
        cf.push_back(correlatedFeatures);
    }
}

//vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
//    std::vector<std::string> nameOfParameters = ts.readParameters();
//    //set shortcut to the map.
//    std::map<std::string , std::vector<float>> param=ts.getParameters();
//    std::vector<AnomalyReport> anomalyReport;
//    for (int i = 0; i < ts.getColumnSize(nameOfParameters[0]); ++i) {
//
//        for (correlatedFeatures correlatedFeatures:cf) {
//            Point point(param[correlatedFeatures.feature1][i] , param[correlatedFeatures.feature2][i]);
//            Circle minCircle = Circle(Point(correlatedFeatures.centerX , correlatedFeatures.centerY) , correlatedFeatures.radius);
//            if(correlatedFeatures.corrlation > threshold  && correlatedFeatures.threshold <= dev(point,correlatedFeatures.lin_reg)){
//                anomalyReport.push_back(AnomalyReport
//                                                (correlatedFeatures.feature1 + "-"+ correlatedFeatures.feature2 ,i+1));
//            } else if(!isPointInCircle((Point &) point, (Circle &) minCircle)&& correlatedFeatures.corrlation < threshold){
//                anomalyReport.push_back(AnomalyReport
//                                                (correlatedFeatures.feature1 + "-"+ correlatedFeatures.feature2 ,i+1));
//            }
//        }
//    }
//    return anomalyReport;
//}


vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    std::vector<std::string> nameOfParameters = ts.readParameters();
    //set shortcut to the map.
    std::map<std::string , std::vector<float>> param=ts.getParameters();
    std::vector<AnomalyReport> anomalyReport;
    for (correlatedFeatures correlatedFeatures:cf) {

            for (int i = 0; i < ts.getColumnSize(nameOfParameters[0]); ++i){
            Point point(param[correlatedFeatures.feature1][i] , param[correlatedFeatures.feature2][i]);
            Circle minCircle = Circle(Point(correlatedFeatures.centerX , correlatedFeatures.centerY) , correlatedFeatures.radius);
            if(correlatedFeatures.corrlation > threshold  && correlatedFeatures.threshold <= dev(point,correlatedFeatures.lin_reg)){
                anomalyReport.push_back(AnomalyReport
                                                (correlatedFeatures.feature1 + "-"+ correlatedFeatures.feature2 ,i+1));
            } else if(!isPointInCircle((Point &) point, (Circle &) minCircle)&& correlatedFeatures.corrlation < threshold){
                anomalyReport.push_back(AnomalyReport
                                                (correlatedFeatures.feature1 + "-"+ correlatedFeatures.feature2 ,i+1));
            }
        }
    }
    return anomalyReport;
}

