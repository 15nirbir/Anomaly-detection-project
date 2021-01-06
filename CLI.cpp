#include "CLI.h"
#include <vector>
CLI::CLI(DefaultIO* dio) {
   CLI::dio = dio;
}

void CLI::start(){
   Data data;
   data.threshold = 0.9;
    std::vector<Command*> commands;
    auto element1 = new UploadCommand(dio);
    commands.push_back(element1);
    auto element2 = new AlgoSettings(dio);
    commands.push_back(element2);
    auto element3 = new DetectAnomalies(dio);
    commands.push_back(element3);
    auto element4 = new DisplayResult(dio);
    commands.push_back(element4);
    auto element5 = new UploadAnomalies(dio);
    commands.push_back(element5);
    auto element6 = new Exit(dio);
    commands.push_back(element6);


    while(true){
        printMenu(&commands[0] , 6);
        std::string choose = dio->read();
        int num = std::stoi(choose) -1;
        commands[num]->execute(data);

        if(num == 5){
            break;
        }
    }
    delete element1;
    delete element2;
    delete element3;
    delete element4;
    delete element5;
    delete element6;
}


CLI::~CLI() {
}

void CLI::printMenu(Command **command , int size) {
    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    for(int i=0;i < size;i++){
        std:: string num = std::to_string(i+1);
        dio->write(num);
        dio->write(".");
        std::string description = command[i]->getDescription();
        dio->write(description);
    }
}

