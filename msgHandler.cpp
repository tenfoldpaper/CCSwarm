#include "./headers/msgHandler.hpp"

using namespace std;
msgHandler::msgHandler(){
    toPort = 0;
    fromPort = 1;
    toSocket = nullptr;
    heartSocket = nullptr;
    fromSocket = nullptr; 
    direction = false;
    cout << "Default msgHandler created." << endl;
    
}

msgHandler::msgHandler(int from_, 
                       int to_, 
                       zmq::socket_t fromS_, 
                       zmq::socket_t toS_,
                       zmq::socket_t hb_, 
                       bool d_){
    
    fromPort = from_;
    toPort = to_;
    direction = d_;
    fromSocket = fromS_;
    heartSocket = hb_;
    toSocket = toS_; 
    cout << "Handler will handle data coming from port " << fromPort << 
            "to " << toPort << endl;
    
    
}


bool msgHandler::insertMsg(string msg){
    if(!isEmpty()){
        setEmpty(true);
    }
    
    msgQueue.push_back(msg); //replace this with inserting the data to SQLite 
    cout << msg << " " << msg.length() << endl;
    return true; 
}

bool msgHandler::handleMsg(){ //permanent loop meant to be run on its own thread
    //Could consider separating this, since the if/else loop is defined 
    //at start and only needs to be confirmed once. 
    cout << "Needs testing! Like a lot!" << endl;
    string heartMsg;
    string msg;
    zmq::pollitem_t items[] = {
        {*fromSocket, 0, ZMQ_POLLIN, 0}, 
        {*heartSocket, 0, ZMQ_POLLIN, 0}
    };
    while(1){
        if(!direction){ //going twd. ccs 
            s_send(*heartSocket, "");
            //wait until receive is done; otherwise terminate
            heartMsg = s_recv(*heartSocket);
            //Set the RX code so that after getting HB, wait a sec. 
            msg = s_recv(*fromSocket); 
            insertMsg(msg);
            s_send(*toSocket, msg);
        }
        else{
            //consider using the normal recv method. could be more robust. 
            heartMsg = s_recv(*heartSocket);
            s_send(*heartSocket, "");
            msg = s_recv(*fromSocket);
            insertMsg(msg);
            s_send(*toSocket, msg);
            //wait until we get a request from the TX. 
        }
        
        
        
    }
    
    
    //Push the message to the local database
     
    return false;
}

void msgHandler::printInfo(){
    cout << "Message handler info: \n"
         << "   toPort: " << toPort 
         << "\n   fromPort: " << fromPort 
         << "\n   direction: " << direction 
            << endl;
};

bool msgHandler::isEmpty(){
    return empty;
}
void msgHandler::setEmpty(bool b){
    empty = b;
}


