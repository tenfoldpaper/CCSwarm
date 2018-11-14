#include "./headers/zhelpers.hpp"
#include <iostream>
#include "./headers/msgHandler.h.hpp"
#include "./headers/comParser.h"
#include <boost/program_options.hpp>
#include <stdexcept> 

namespace po = boost::program_options;
using namespace std;

int main(int argc, char* argv[]){
    
    cout << "Welcome to CCSwarm Hub." << endl;
    
    string ccs5SubIP, satPubIP, ccs5PubIP, satSubIP, extraPubIP;
    string satPubHBIP, satSubHBIP; //Heartbeat IP addresses for the satellite 
    
    int ccs5SubPort, satPubPort, ccs5PubPort, satSubPort, extraPubPort;
    int satPubHBPort, satSubHBPort;
    
    string portPrefix = "tcp://127.0.0.1:" ;
    bool success = false; 
    bool extraFlag = false;
    
    try{
        
        po::options_description desc("Usage: ccswarm [option] ?[arg] ...");
        desc.add_options()
            ("help", "Displays this message.")
            ("ccs5sub", po::value<int>(), "Sets the port number for the socket subscribing to the CCS5's output. Default is 7770.")
            ("satpub", po::value<int>(), "Sets the port number for the socket publishing to the satellite's RX model. Default is 7771.")
            ("satpubheart", po::value<int>(), "Sets the port number for the heartbeat node of the satellite's RX model. Default is 7781.")
            ("satsub", po::value<int>(), "Sets the port number for the socket subscribing to the satellite's TX model. Default is 7772.")
            ("satsubheart", po::value<int>(), "Sets the port number for the heartbeat node of the satellite's TX model. Default is 7782.")
            ("ccs5pub", po::value<int>(), "Sets the port number for the socket publishing to the CCS5's input. Default is 7773.")
            ("extra", po::value<int>(), "Sets the port number for the socket that additional clients can subscribe to. Default is 8800.")
            ("extra-on", "Enables the extra port to which additional clients can subscribe in order to receive monitoring data.")
        ;
        
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        
        if(vm.count("help")){
            cout << desc << "\n";
            return 0;
        }
        if(vm.count("extra-on")){
            extraFlag = true;
            cout << "Extra port has been enabled." << endl;
        }
        if(vm.count("ccs5sub")){
            ccs5SubPort = vm["ccs5sub"].as<int>();
            if(ccs5SubPort > 65535 || ccs5SubPort < 1){
                throw(invalid_argument("invalid port number"));
            }
            cout << "Port of the socket receiving data from CCS5 has been set to "
                 << ccs5SubPort << ".\n";
            ccs5SubIP = portPrefix + to_string(ccs5SubPort);
             cout << "    IP: " << ccs5SubIP << "\n";
        }
        else{
            cout << "Port of the socket receiving data from CCS5 has been set to "
                 << "its default value of 7770.\n";
            ccs5SubIP = "tcp://127.0.0.1:7770";
            cout << "    IP: " << ccs5SubIP << "\n";
            ccs5SubPort = 7770;
        }
        
        if(vm.count("satpub")){
            satPubPort = vm["satpub"].as<int>();
            if(satPubPort > 65535 || satPubPort < 1){
                throw(invalid_argument("invalid port number"));
            }
            cout << "Port of the socket sending data to swarmFLP has been set to "
                 << satPubPort << ".\n";
            satPubIP = portPrefix + to_string(satPubPort);
             cout << "    IP: " << ccs5PubIP << "\n";
        }
        else{
            cout << "Port of the socket sending data to swarmFLP has been set to "
                 << "its default value of 7771.\n";
            satPubIP = "tcp://127.0.0.1:7771";
            cout << "    IP: " << satPubIP << "\n";
            satPubPort = 7771;
        }
        //
        if(vm.count("satpubheart")){
            satPubHBPort = vm["satpubheart"].as<int>();
            if(satPubHBPort > 65535 || satPubHBPort < 1){
                throw(invalid_argument("invalid port number"));
            }
            cout << "Port of the heartbeat socket of RX has been set to "
                 << satPubHBPort << ".\n";
            satPubHBIP = portPrefix + to_string(satPubHBPort);
             cout << "    IP: " << satPubHBIP << "\n";
        }
        else{
            cout << "Port of the heartbeat socket of RX has been set to "
                 << "its default value of 7781.\n";
            satPubHBIP = "tcp://127.0.0.1:7781";
            cout << "    IP: " << satPubHBIP << "\n";
            satPubHBPort = 7781;
        }
        
        //
        if(vm.count("satsub")){
            satSubPort = vm["satsub"].as<int>();
            if(satSubPort > 65535 || satSubPort < 1){
                throw(invalid_argument("invalid port number"));
            }
            cout << "Port of the socket receiving data from swarmFLP has been set to "
                 << satSubPort << ".\n";
            satSubIP = portPrefix + to_string(satSubPort);
            cout << "    IP: " << satSubIP << "\n";
        }
        else{
            cout << "Port of the socket receiving data from swarmFLP has been set to "
                 << "its default value of 7772.\n";
            satSubIP = "tcp://127.0.0.1:7772";
            cout << "    IP: " << satSubIP << "\n";
            satSubPort = 7772;
        }
        
        if(vm.count("satsubheart")){
            satSubHBPort = vm["satsubheart"].as<int>();
            if(satSubHBPort > 65535 || satSubHBPort < 1){
                throw(invalid_argument("invalid port number"));
            }
            cout << "Port of the heartbeat socket of TX has been set to "
                 << satSubHBPort << ".\n";
            satSubHBIP = portPrefix + to_string(satSubHBPort);
            cout << "    IP: " << satSubHBIP << "\n";
        }
        else{
            cout << "Port of the heartbeat socket of TX has been set to "
                 << "its default value of 7782.\n";
            satSubHBIP = "tcp://127.0.0.1:7782";
            cout << "    IP: " << satSubHBIP << "\n";
            satSubHBPort = 7782;
        }
        
        if(vm.count("ccs5pub")){
            ccs5PubPort = vm["ccs5pub"].as<int>();
            if(ccs5PubPort > 65535 || ccs5PubPort < 1){
                throw(invalid_argument("invalid port number"));
            }
            cout << "Port of the socket sending data to CCS5 has been set to "
                 << ccs5PubPort << ".\n";
            ccs5PubIP = portPrefix + to_string(ccs5PubPort);
            cout << "    IP: " << ccs5PubIP << "\n";
            
        }
        else{
            cout << "Port of the socket sending data to CCS5 has been set to "
                 << "its default value of 7773.\n";
            ccs5PubIP = "tcp://127.0.0.1:7773";
            cout << "    IP: " << ccs5PubIP << "\n";
            ccs5PubPort = 7773;
        }
        if(extraFlag){
            if(vm.count("extra")){
                extraPubPort = vm["extra"].as<int>();
                if(extraPubPort > 65535 || extraPubPort < 1){
                    throw(invalid_argument("invalid port number"));
                }
                cout << "Port of the extra socket sending data has been set to "
                     << extraPubPort << ".\n";
                extraPubIP = portPrefix + to_string(extraPubPort);
            }
            else{
                cout << "Port of the extra socket sending data has been set to "
                     << "its default value of 8800.\n";

                extraPubIP = "tcp://127.0.0.1:8800";
                cout << "IP: " << extraPubIP << "\n";
                extraPubPort = 8800;
            }
        }
    }
    catch(const invalid_argument& e){
        cerr << "error: Port number too large\n";
        return 2;
    }
    
    catch(exception& e){
        cerr << "error: " << e.what() << "\n";
        cout << "Refer to the --help option." << "\n";
        return 1;
    }
    
    catch(...){
        cerr << "Exception of unknown type!\n";
        cout << "Refer to the --help option." << "\n";
        return -1;
    }

    zmq::context_t context(1);
    //Part that connects to CCS5.  
    zmq::socket_t CCS5_subscriber(context, ZMQ_STREAM);
    zmq::socket_t CCS5_publisher(context, ZMQ_STREAM);
    
    //Part that connects to swarmFLP.
    zmq::socket_t SAT_publisher(context, ZMQ_PUB);  
    zmq::socket_t SAT_subscriber(context, ZMQ_SUB);
    SAT_subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    
    //Part that sends empty heartbeat messages back to the satellite. 
    zmq::socket_t SAT_pubHeart(context, ZMQ_REP);
    zmq::socket_t SAT_subHeart(context, ZMQ_REP);
    
    //Part that enables additional clients to receive data. 
    if(extraFlag){
        zmq::socket_t observer_publisher(context, ZMQ_PUB);
    }
    
    cout << "Handling binding and connecting " << endl;
    CCS5_subscriber.bind(ccs5SubIP);
    SAT_publisher.bind(satPubIP);
    SAT_pubHeart.bind(satPubHBIP);
    SAT_subHeart.bind(satSubHBIP);

    msgHandler *COMHandler = new msgHandler(ccs5SubPort, 
                                            satPubPort,
                                            CCS5_subscriber, 
                                            SAT_publisher,
                                            SAT_pubHeart,
                                            false); //Handles COM msg from CCS5
    
    msgHandler *PayloadHandler = new msgHandler(satSubPort,
                                                ccsPubPort,
                                                SAT_subscriber,
                                                CCS5_publisher,
                                                SAT_subHeart,
                                                true
                                                ); //Handles Payload from SIM
    
    std::cout << "Starting the loop" << endl;
    while(1){
        cout << "Listening to COM from CCS5..." << endl;
        string header = s_recv(CCS5_subscriber);
        string payload = s_recv(CCS5_subscriber);
        
        cout << "COM Received" << endl;
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
        int plLen = payload.length();
        zmq::message_t toSatRXPayload(plLen);
        memcpy((void*)toSatRXPayload.data(), payload.c_str(), plLen);
        
        while(!success){//Is retrying until success a good strategy here? 
            success = SAT_publisher.send(toSatRXPayload);
            if(!success){
                cout << "Publishing the data to SAT was not successful. Retrying..." << endl;
                sleep(1); //Don't overload the computer with infinite attempts
            }
        }
        
        sleep(1);
    }
    return 0;
}