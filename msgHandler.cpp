#include "./headers/msgHandler.hpp"

using namespace std;
msgHandler::msgHandler(){
    std::cout << "Default msgHandler created." << std::endl;
}

msgHandler::msgHandler(int pNum_){
    portNumber = pNum_;
    cout << "msgHandler bound to port " << portNumber << " created. " << std::endl;
}

bool msgHandler::receiveMsg(string msg){
    if(!isEmpty()){
        setEmpty(true);
    }
    
    msgQueue.push_back(msg);
    cout << msg << " " << msg.length() << endl;
    return true; 
}

bool msgHandler::handleMsg(){
    cout << "Not yet implemented!" << endl;
    //Push the message to the local database
    
    //Send the message to the SAT 
    
}

void msgHandler::printInfo(){};

bool msgHandler::isEmpty(){
    return empty;
}
void msgHandler::setEmpty(bool b){
    empty = b;
}
