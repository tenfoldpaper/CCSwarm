#include "./headers/zhelpers.hpp"
#include <iostream>
#include "./headers/msgHandler.hpp"
#include "./headers/comParser.h"
using namespace std;

int main(int argc, char* argv[]){
    
    cout << "Welcome to CCSwarm Hub." << endl;
    
    string ccs5SubIP, satPubIP;
    string portPrefix = "tcp://localhost:" ;
    bool success = false; 
    

    if(argc != 3){
        cout << "Argument count is not 2. Using default port values." << endl;
        cout << "CCS5 Subscriber port: 7770" << endl;
        cout << "SAT publisher port: 7771" << endl;
        ccs5SubIP = "tcp://127.0.0.1:7770";
        satPubIP = "tcp://127.0.0.1:7771";
    }
    else{
        ccs5SubIP = argv[1];
        satPubIP = argv[2]; 
        cout << "Argument count is 2. Using them as port values." << endl;
        cout << "CCS5 subscriber port: " << argv[1] << endl;
        cout << "SAT publisher port: " << argv[2] << endl;
        ccs5SubIP = portPrefix + ccs5SubIP;
        satPubIP = portPrefix + satPubIP;
        //debug
        cout << ccs5SubIP << satPubIP << endl;
    }

    zmq::context_t context(1);
    //Part that connects to CCS5.  
    zmq::socket_t CCS5_subscriber(context, ZMQ_STREAM);
    zmq::socket_t SAT_publisher(context, ZMQ_PUB);  
    //Servers bind. Clients connect. 
    //CCSwarm-related ports will always be 77xx. 
    cout << "Handling binding and connecting " << endl;
    CCS5_subscriber.bind(ccs5SubIP);
    SAT_publisher.connect(satPubIP);

    msgHandler *COMHandler = new msgHandler(7770); //Handles COM msg from CCS5
    msgHandler *PayloadHandler = new msgHandler(7772); //Handles Payload from SIM
    
    std::cout << "Starting the loop" << endl;
    while(1){
        cout << "Listening to COM from CCS5..." << endl;
        std::string header = s_recv(CCS5_subscriber);
        std::string payload = s_recv(CCS5_subscriber);
        std::cout << "COM Received" << endl;
        //cout << "Header:" << header << " " <<  endl;
        cout << "Payload:" << payload << " " << endl; 
        cout << "Handling the data... " << endl;
        COMHandler->receiveMsg(payload);
        //Start a new thread, and:
        //      1. Parse the message 
        //      2. Put the message to a queue 
        //      3. Create a bit vector to indicate pub successes
        //      4. Send the message; if 1, flip bit; else retry til timeout
        //      5. Repeat 4 for all the commands
        //      6. Print bit state and kill the thread  
        //Handle the data for storing to the database  
        //      1. Store the COM + publishing success together to the RelDB
        //
        while(!success){//Is retrying until success a good strategy here? 
            success = s_send(SAT_publisher, payload);
            if(!success){
                cout << "Publishing the data to SAT was not successful. Retrying..." << endl;
                sleep(1); //Don't overload the computer with infinite attempts
            }
        }
        
        sleep(1);
    }
    return 0;
}