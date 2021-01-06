cmake_minimum_required(VERSION 3.17)
project(TimeSeries)

set(CMAKE_CXX_STANDARD 11)

add_executable(timeSeries main.cpp TimeSeries.cpp TimeSeries.h anomaly_detection_util.cpp anomaly_detection_util.h
        SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h AnomalyDetector.h minCircle.cpp
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h minCircle.h CLI.cpp CLI.h commands.h Server.cpp Server.h)