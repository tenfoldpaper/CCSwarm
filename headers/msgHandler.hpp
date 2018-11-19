
/*
 * The message handler is a unidirectional message conveyer. 
 * It continuously listens to the fromPort, and then conveys the message 
 * to the other side. It also handles the INSERT function of the local 
 * SQLite database. 
 *   
 */
#ifndef MSGHANDLER_HPP
#define MSGHANDLER_HPP

#include <iostream>
#include <vector>
#include <string> 
#include <stdio.h>
#include <string>
#include <zmq.hpp>
#include "./headers/zhelpers.hpp"

class msgHandler {

private:
    std::vector<std::string> msgQueue; 
    int toPort; //Specifies the port the msgHander sends the data to 
    int fromPort; //Specifies the port the msgHandler receives data from 
    bool empty; 
    zmq::socket_t* fromSocket; 
    zmq::socket_t* heartSocket;
    zmq::socket_t* toSocket; 
    bool direction; //0: to Sat; 1: to CCS

public:
    msgHandler();

    msgHandler(int, zmq::socket_t*, zmq::socket_t*, zmq::socket_t*);

    ~msgHandler();
    
    bool insertMsg(std::string);//Pushes the msg in the arg to SQLite db 
    
    bool handleMsg();//Goes through the entire queue, storing it into the database. 

    void printInfo();//Prints out the current status 
    
    int getPortNumber();
    
    bool isEmpty();
    
    void setEmpty(bool);
    
};

#endif 