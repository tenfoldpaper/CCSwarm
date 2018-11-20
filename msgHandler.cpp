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

msgHandler::msgHandler(zmq::socket_t* fromS_, int fromPort_,
                       zmq::socket_t* toS_, int toPort_,
                       zmq::socket_t* hb_, int hbPort_,
                       bool d_){
    
    fromPort = fromPort_;
    toPort = toPort_;
    hbPort = hbPort_;
    direction = d_;
    fromSocket = fromS_;
    heartSocket = hb_;
    toSocket = toS_; 
    if(direction){
        label = 'S';
    }
    else{
        label = 'C';
    }
    cout << label << ": from " << fromPort << " to " << toPort << " with heart"
            << hbPort << endl;
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
    
    return true; 
}

//Add error handling to this 
bool msgHandler::handleMsgA(){ //permanent loop meant to be run on its own thread
    //This handler receives STREAM msg from CCS5, and sends it to 
    //the satellite in ZMQ_PUB format after checking if it is alive using
    //ZMQ_REQ. 
    
    cout << "Needs testing! Like a lot!" << endl;
    string heartMsg;
    bool sendSuccess = false;
    zmq::pollitem_t items[] = {
        {static_cast<void*>(*fromSocket), 0, ZMQ_POLLIN, 0}, 
        {static_cast<void*>(*heartSocket), 0, ZMQ_POLLIN, 0}
    };
    sleep(0.5);
    while(whileFlag){
        try{
            
            cout << label << ": Waiting for a message from " << fromPort << endl; 
           string recvMsg = s_recv(*fromSocket);
            cout << label << ": Received: " << recvMsg << endl;
            cout << label << ": Check: " << recvMsg << endl;
            
            cout << label << ": Checking if the heart is alive at " << hbPort << endl;
            s_send(*heartSocket, "");
            //wait until receive is done; otherwise terminate
            heartMsg = s_recv(*heartSocket);
            cout << label << ": Heart is alive. Sending msg to socket. " << endl;
            //Set the RX code so that after getting HB, wait a sec. 
            
            insertMsg(recvMsg);
            s_send(*toSocket, recvMsg);
            cout << label << ": Pushing the message to a database" << endl;
        }
        catch(exception& e){
            cout << label << ": Server is unavailable." << endl;
            cout << e.what() << endl;
            return false;
        }
    } 
    //Push the message to the local database
     
    return false;
}

bool msgHandler::handleMsgB(){ //permanent loop meant to be run on its own thread
 
    cout << "\nNeeds testing! Like a lot!" << endl;
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
            cout << label << ": Listening for a heartbeat at " << hbPort << endl;
            //consider using the normal recv method. could be more robust. 
            heartMsg = s_recv(*heartSocket);
            s_send(*heartSocket, "");
            cout << label << ": Waiting for a message from " << fromPort << endl;
            
            string recvMsg = s_recv(*fromSocket);
            
            cout << label << ": Received: " << recvMsg << endl;

            cout << label << ": Sending message to " << toSocket << endl;
            s_send(*toSocket, recvMsg);
            //wait until we get a request from the TX. 
            cout << label << ": Pushing the message to a database" << endl;
        }
        catch(exception& e){
            cout << label << ": Server is unavailable." << endl;
            cout << e.what() << endl;
            return false;
        }
    } 
    //Push the message to the local database
     
    return false;
}
   

void msgHandler::printInfo(){
    cout << label << ": Message handler info: \n"
         << "   toPort:    " << toPort 
         << "\n   fromPort:  " << fromPort 
         << "\n   hbPort:    "
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


