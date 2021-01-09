/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <string.h>
#include "CLI.h"
#include <signal.h>
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{

    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class SocketIO:public DefaultIO{
    int clientID;
public:
    SocketIO(int clientID);
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{

	public:
    virtual void handle(int clientID){
        SocketIO *dio = new SocketIO(clientID);
        CLI *cli = new CLI(dio);
        cli->start();
        delete dio;
        delete cli;
    }
};


// implement on Server.cpp
class Server {
    bool isRun;
	thread* t; // the thread to run the start() method in
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    void setIsRun();
	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
