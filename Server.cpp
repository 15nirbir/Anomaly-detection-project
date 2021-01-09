
#include "Server.h"

Server::Server(int port)throw (const char*) {
    isRun=true;
    fd = socket(AF_INET , SOCK_STREAM,0);
    if(fd<0)
        throw "socket failed";

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if(::bind(fd , (struct sockaddr*)&server , sizeof(server)) < 0){
        throw "bind failed";
    }
    if(listen(fd , 3)<0)
        throw "listen failed";
}

void Server::start(ClientHandler& ch)throw(const char*){

    t = new thread([&ch , this] () {
        while(isRun) {

        //std::cout << "waiting for a client" << std::endl;

            socklen_t clientSize = sizeof(client);
            alarm(3);

            int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
            if (aClient < 0) {
                throw "accept failed";
            }
            //std::cout << "client connected!" << std::endl;
            ch.handle(aClient);
            close(aClient);
            sleep(2);
        }
        close(fd);


    });

    }


void Server::stop(){
    isRun =false;
    t->join(); // do not delete this!
}

Server::~Server() {
}

void Server::setIsRun() {
    isRun = false;
}

string SocketIO::read() {

    char buffer[256];
   bzero(buffer,256);
   char temp[256];
    bzero(temp,256);
   int i=0;
   while(true){
       ::read(clientID , buffer , 1);
       if(buffer[0] == '\n')
           break;
       temp[i] = buffer[0];

       i++;
   }

    return temp;
}

SocketIO::SocketIO(int clientID) {
    this->clientID = clientID;
}

void SocketIO::write(string text) {
    char t[text.size()+1];
    strcpy(t , text.c_str());
    ::send(clientID ,t,text.size() , 0);
}
//maybe will be a problem with to much digit after point.
void SocketIO::write(float f) {
//    float x = floorf((float)f*1000)/1000;
//    std:: string num = std:: to_string(x);
//    cout << "num is " << num << endl;
    std:: ostringstream x;
    x << f;
    std:: string num(x.str());

    char t[num.size()];
    strcpy(t , num.c_str());
    ::send(clientID , t , num.size() , 0);

}
//maybe the client will read more then 2048 bytes.
void SocketIO::read(float *f) {

    recv(clientID , f , 2048 , 0);
}


