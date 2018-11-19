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
                       zmq::socket_t* fromS_, 
                       zmq::socket_t* toS_,
                       zmq::socket_t* hb_, 
                       bool d_){
    
    fromPort = from_;
    toPort = to_;
    direction = d_;
    fromSocket = fromS_;
    heartSocket = hb_;
    toSocket = toS_; 
    cout << "Handler will handle data coming from port " << fromPort << 
            " to " << toPort << endl; 
    whileFlag = true;
        
}

msgHandler::~msgHandler(){
    cout << "Destroying the msgHandler\n" << endl;
   
}


bool msgHandler::insertMsg(string msg){
    if(isEmpty()){
        setEmpty(false);
    }
    
    msgQueue.push_back(msg); //replace this with inserting the data to SQLite 
    cout << msg << " " << msg.length() << endl;
    return true; 
}

//Add error handling to this 
bool msgHandler::handleMsgA(){ //permanent loop meant to be run on its own thread

    cout << "Needs testing! Like a lot!" << endl;
    string heartMsg;
    string msg;
    bool sendSuccess = false;
    zmq::pollitem_t items[] = {
        {static_cast<void*>(*fromSocket), 0, ZMQ_POLLIN, 0}, 
        {static_cast<void*>(*heartSocket), 0, ZMQ_POLLIN, 0}
    };
    sleep(0.5);
    while(whileFlag){
        try{
            cout << "Checking if the heart is alive at " << toPort + 1 << endl;
            s_send(*heartSocket, "");
            //wait until receive is done; otherwise terminate
            heartMsg = s_recv(*heartSocket);
            //Set the RX code so that after getting HB, wait a sec. 
            msg = s_recv(*fromSocket); 
            insertMsg(msg);
            s_send(*toSocket, msg);
            cout << "Pushing the message to a database" << endl;
        }
        catch(exception& e){
            cout << "Server is unavailable." << endl;
            cout << e.what() << endl;
            return false;
        }
    } 
    //Push the message to the local database
     
    return false;
}

bool msgHandler::handleMsgB(){ //permanent loop meant to be run on its own thread
 
    cout << "Needs testing! Like a lot!" << endl;
    string heartMsg;
    string msg;
    bool sendSuccess = false;
    zmq::pollitem_t items[] = {
        {static_cast<void*>(*fromSocket), 0, ZMQ_POLLIN, 0}, 
        {static_cast<void*>(*heartSocket), 0, ZMQ_POLLIN, 0}
    };
    sleep(0.5);
    while(whileFlag){
        try{
            cout << "Listening for a heartbeat at " << fromPort + 1 << endl;
            //consider using the normal recv method. could be more robust. 
            heartMsg = s_recv(*heartSocket);
            s_send(*heartSocket, "");
            msg = s_recv(*fromSocket);
            insertMsg(msg);
            s_send(*toSocket, msg);
            //wait until we get a request from the TX. 
            cout << "Pushing the message to a database" << endl;
        }
        catch(exception& e){
            cout << "Server is unavailable." << endl;
            cout << e.what() << endl;
            return false;
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

void msgHandler::setWhileFlag(bool b){
    whileFlag = b;
}


