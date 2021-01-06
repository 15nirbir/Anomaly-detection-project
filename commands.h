

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <list>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes
struct AnomalyEvent{
    std:: string description;
    int startLine;
    int endLine;
};
struct Data {
    TimeSeries *ts1;
    TimeSeries *ts2;
    float threshold;
    HybridAnomalyDetector *detector;
    std:: vector <AnomalyReport> r;
   std:: vector <AnomalyEvent> anomalyEvent;
   std:: vector <AnomalyEvent> detectFile;
   int numberOfLines;
   int numberOfAlarmLine;
};


// you may edit this class
class Command{

protected:
    DefaultIO* dio;
    std::string description;
    int numberOfFalsePositive(Data& data){
        int counter = 0;
        bool isAnomaly = false;
        for(AnomalyEvent detector:data.anomalyEvent){
            for(AnomalyEvent real:data.detectFile){
                if(!(detector.startLine > real.endLine || detector.endLine < real.startLine))
                  isAnomaly=true;
            }
            if(isAnomaly)
                counter++;
        }
        return counter;
    }
    int numberOfTruePositive(Data& data){
        int counter = 0;
        bool isAnomaly = false;
        for(AnomalyEvent real:data.detectFile) {
            for(AnomalyEvent detector:data.anomalyEvent){
                if(((detector.startLine >= real.startLine && detector.startLine <= real.endLine)||
                        (detector.endLine <= real.endLine&&detector.endLine >= real.startLine)||
                        (detector.startLine <= real.startLine && detector.endLine >= real.endLine)))
                    isAnomaly=true;
            }
            if(isAnomaly)
                counter++;
        }
        return counter;
    }
    void setDetectFile(Data& data){
        std:: string line;
        std:: vector <AnomalyEvent> anomalys;
        int anomalylines = 0;
        do{
            line = dio->read();

            if(line.compare("done") == 0){
                break;
            }
            std::stringstream lineStream(line);
            std::string cell;
            int i=0;
            AnomalyEvent temp;
            while (std::getline(lineStream, cell, ',')){
                if(i==0){
                    temp.startLine = std::stoi(cell);
                } else
                    temp.endLine = std::stoi(cell);
                i++;
            }
            anomalylines += temp.endLine - temp.startLine +1;
            anomalys.push_back(temp);
        }while(true);
        data.detectFile = anomalys;
        data.numberOfAlarmLine = anomalylines;
    }
    void setAnomalys(Data& data){
        std:: vector <AnomalyEvent> anomalys;
        int tempTimeStep;
        int counter=0;
        for(AnomalyReport anomalyReport:data.r){
            counter++;
            if(anomalys.empty() == 1){
                AnomalyEvent newAnomaly;
                newAnomaly.description = anomalyReport.description;
                newAnomaly.startLine = anomalyReport.timeStep;
                tempTimeStep = anomalyReport.timeStep;
                anomalys.push_back(newAnomaly);
                // TO-DO handle the last report.
                if(counter == data.r.size()){
                    anomalys.back().endLine = tempTimeStep;
                }
            } else if(anomalyReport.description.compare(anomalys.back().description) != 0
            || tempTimeStep + 1 != anomalyReport.timeStep){
                anomalys.back().endLine = tempTimeStep;
                AnomalyEvent newAnomaly;
                newAnomaly.description = anomalyReport.description;
                newAnomaly.startLine = anomalyReport.timeStep;
                tempTimeStep = anomalyReport.timeStep;
                anomalys.push_back(newAnomaly);
                if(counter == data.r.size()){
                    anomalys.back().endLine = tempTimeStep;
                }
            } else{
                tempTimeStep++;
                if(counter == data.r.size()){
                    anomalys.back().endLine = tempTimeStep;
                }

            }

        }
        data.anomalyEvent = anomalys;
    }

public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute(Data& data)=0;
	virtual ~Command(){}
    std::string& getDescription(){
	    return description;
	}
	void makeTS(char* fileName , Data& data , bool isLearn){
	    ofstream file;
	    file.open(fileName);
	    std:: string line;
	    int numOfRows=0;
	    do{
	        line = dio->read();
	        if(line.compare("done")==0){
                break;
	        }
	        numOfRows++;
	        file << line;
	        file << "\n";
	    } while(true);
	    file.close();
	    data.numberOfLines = numOfRows;
	    if(isLearn){
	        data.ts1 = new TimeSeries(fileName);
	    } else
	        data.ts2 = new TimeSeries(fileName);
	}
};

// implement here your command classes
class UploadCommand: public Command{
public:
    UploadCommand(DefaultIO* dio):Command (dio){
          description = "upload a time series csv file\n";
        }


    // TO-DO observer.
    void execute(Data& data) override{
        //save the name of trainCsv.
        dio->write("Please upload your local train CSV file.\n");
        //TO-DO write read command.
        makeTS("learn.csv" ,data , 1 );
       dio->write("Upload complete.\n");
        //save the name of testCsv.
        dio->write("Please upload your local test CSV file.\n");
        //TO-DO write read command.
        makeTS("mytest.csv" , data , 0);
        dio->write("Upload complete.\n");
    }
};

class AlgoSettings: public Command{
public:
    AlgoSettings(DefaultIO* dio):Command (dio){
            description = "algorithm settings\n";
    }
    //update threshold.
    void execute(Data& data) override{
       std:: string currentCorrelation = std:: to_string(data.threshold);
       dio->write("The current correlation threshold is ");
       dio->write(data.threshold);
       dio->write("\n");
       dio->write("Type a new threshold\n");
        while(true){
            std::string newCorrelation = dio->read();
            float threshold = std::stof(newCorrelation);
            if(threshold >= 0 && threshold <= 1){
                data.threshold = threshold;
                break;

            }
            else
                dio->write("please choose a value between 0 and 1.");
        }
    }
};

class DetectAnomalies: public Command{
public:
    DetectAnomalies(DefaultIO* dio):Command (dio){
        description = "detect anomalies\n";
    }
    void execute(Data& data) override{
        data.detector = new HybridAnomalyDetector();
       const TimeSeries timeSeries = *(data.ts1);
       const TimeSeries timeSeries2 = *(data.ts2);
       data.detector->setTreshold(data.threshold);
        data.detector->learnNormal(timeSeries);
        data.r = data.detector->detect(timeSeries2);
        dio->write("anomaly detection complete.\n");
    }
};

class DisplayResult: public Command{
public:
    DisplayResult(DefaultIO* dio):Command (dio){
            description = "display results\n";
    }
    void execute(Data& data) override{
        int size = data.r.size();
        for(int i=0;i <size;i++){
            std:: string timeStep = std:: to_string(data.r[i].timeStep);
            dio->write(timeStep);
            dio->write(" ");
            dio->write("\t");
            dio->write(data.r[i].description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

class UploadAnomalies: public Command{
public:
    UploadAnomalies(DefaultIO* dio):Command (dio){
            description = "upload anomalies and analyze results\n";
    }
    void execute(Data& data) override{
        dio->write("Please upload your local anomalies file.\n");
        dio->write("Upload complete.\n");
        setAnomalys(data);
        setDetectFile(data);

        int N = data.numberOfLines - data.numberOfAlarmLine -1;
        int P = data.detectFile.size();
        int TP = numberOfTruePositive(data);
        int FP = data.anomalyEvent.size()-TP;
        dio->write("True Positive Rate: ");
        dio->write(floorf((float)TP/P*1000)/1000);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(floorf((float)FP/N*1000)/1000);
        dio->write("\n");
    }
};

class Exit: public Command{
public:
    Exit(DefaultIO* dio):Command (dio){
            description = "exit\n";
    }
    void execute(Data& data) override{

    }
};


#endif /* COMMANDS_H_ */
