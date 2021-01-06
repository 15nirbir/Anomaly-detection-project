//
// Created by nir birnboim on 25/11/2020.
//

#include "timeseries.h"
    TimeSeries:: TimeSeries(const char* CSVfileName){
        name = CSVfileName;
        readFile();
    }
    //read all the columns to the vector.
    void TimeSeries::readFile() {
        std:: vector<std::string> nameOfParameters;
        nameOfParameters=readParameters();
        std::vector<std::vector<float>> vectorsOfData;
        //init the vector of data with empty vectors
        for(int i=0;i<nameOfParameters.size();i++){
            vectorsOfData.push_back(std::vector<float>());
        }
        std::string line;
        std::ifstream file(name);
        //jump from line 1.
        std::getline(file, line);
        //check the fle opened correctly.
        if (file.is_open()) {

            while(std::getline(file, line)) {
                int i=0;
                std::stringstream lineStream(line);
                std::string cell;
                while (std::getline(lineStream, cell, ',')) {
                    vectorsOfData[i].push_back(std::stof(cell));
                    i++;

                }

            }
            int i=0;
            for(auto param:nameOfParameters){

               parameters.insert( std::pair<std::string,std::vector<float>> (param,vectorsOfData[i]));
                i++;
            }
        }
            file.close();
    }
    //read the name of the parameters to vector.
    std::vector<std::string> TimeSeries::readParameters()const  {
        std::ifstream  file(name);
                std::vector<std::string> result;
                std::string line;
                std::getline(file, line);

                std::stringstream lineStream(line);
                std::string cell;

                while (std::getline(lineStream, cell, ',')) {
                    result.push_back(cell);
                }
            file.close();
            return result;
        }
    std::map<std::string , std::vector<float>> TimeSeries::getParameters()const {
        return parameters;
    }
    float* TimeSeries::getColumn(std::string& str)const {

        return &(getParameters()[str][1]);
    }
    //length of the column in the csv.
    int TimeSeries::getColumnSize(std::string& string)const {

        return parameters.at(string).size();
    }

