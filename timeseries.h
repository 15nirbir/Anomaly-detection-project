//
// Created by nir birnboim on 25/11/2020.
//

#ifndef TIMESERIES_TIMESERIES_H
#define TIMESERIES_TIMESERIES_H
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
class TimeSeries {
private:
    const char* name;
    std::map<std::string , std::vector<float>> parameters;
    void readFile();

public:
    std::vector<std::string> readParameters()const;
    TimeSeries(const char* CSVfileName);
    void addLine(std::string& line);
    std::map<std::string , std::vector<float>> getParameters()const;
    float* getColumn( std::string& str)const;
    int getColumnSize(std::string& str)const;




};


#endif //TIMESERIES_TIMESERIES_H
