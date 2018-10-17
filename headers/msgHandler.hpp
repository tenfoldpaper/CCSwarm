//Header file for the message handler component of the HUB. 
//

#include <iostream>
#include <vector>
#include <string> 
#include <stdio.h>
#include <string.h>

class msgHandler {

private:
    std::vector<std::string> msgQueue; 
    
    int port_number; //Specifies the port the msgHandler receives data from 
    bool empty; 

public:
    msgHandler();

    msgHandler(int);

    ~msgHandler();
    
    bool receiveMsg(std::string);//Pushes the msg in the arg to its internal queue
    
    bool handleMsg();//Goes through the entire queue, storing it into the database. 

    void printInfo();//Prints out the current status 

    bool isEmpty();
    void setEmpty(bool);
    
};
