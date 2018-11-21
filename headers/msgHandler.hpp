
/*
 * The message handler is a unidirectional message conveyer. 
 * It continuously listens to the fromSocket, and then conveys the message 
 * to the toSocket. It also handles the INSERT function of the local 
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
#include "./zhelpers.hpp"
#include <fstream>

class msgHandler {

private:
    std::vector<std::string> msgQueue; 
    int toPort; //Specifies the port the msgHander sends the data to 
    int fromPort; //Specifies the port the msgHandler receives data from 
    int hbPort;
    bool empty; 
    zmq::socket_t* fromSocket; 
    zmq::socket_t* heartSocket;
    zmq::socket_t* toSocket; 
    char label;
    bool direction; //0: to Sat; 1: to CCS
    bool whileFlag;

public:
    msgHandler();

    msgHandler(zmq::socket_t*, int, 
               zmq::socket_t*, int, 
               zmq::socket_t*, int, bool);

    ~msgHandler();
    
    bool insertMsg(std::string);//Pushes the msg in the arg to SQLite db 
    
    //For direction: publishing to satellite 
    bool handleMsgA();//Goes through the entire queue, storing it into the database. 
    
    //For direction: subscribing to satellite 
    bool handleMsgB(); //For direction == 1;
    
    void printInfo();//Prints out the current status 
    
    bool isEmpty();
    
    void setEmpty(bool);
    
    void setWhileFlag(bool);
    
};

#endif 