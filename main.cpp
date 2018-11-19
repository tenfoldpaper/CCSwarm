
#include <zhelpers.hpp>
#include <iostream>
#include "./headers/msgHandler.hpp"
#include "./headers/comParser.h"
#include <boost/program_options.hpp>
#include <stdexcept> 
#include <thread>

namespace po = boost::program_options;
using namespace std;

int main(int argc, char* argv[]){
    
    cout << "Welcome to CCSwarm Hub." << endl;
    
    string ccs5SubIP, satPubIP, ccs5PubIP, satSubIP, extraPubIP;
    string satPubHBIP, satSubHBIP; //Heartbeat IP addresses for the satellite 
    
    int ccs5SubPort, satPubPort, ccs5PubPort, satSubPort, extraPubPort;
    int satPubHBPort, satSubHBPort;
    
    thread comThread, pldThread;
    msgHandler* COMHandler;
    msgHandler* PayloadHandler;
    //do we need multiple portprefixes? 
    string portPrefix;
    
    bool extraFlag = false;
    bool comhandlerON = false, pldhandlerON = false;
    
    /* options */
    try{
        
        po::options_description desc("Usage: ccswarm [option] ?[arg] ...");
        desc.add_options()
            ("help", "Displays this message.")
            ("ip", po::value<string>(), "Sets the IP address. Protocol needs to be ZMQ-compliant (tcp, inproc, ipc, pgm, epgm), or else the program will fail. e.g. \"tcp://127.0.0.1\"")
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
        if(vm.count("ip")){
            
            portPrefix = vm["ip"].as<string>() + ":";
            if(portPrefix.find("tcp") != string::npos || 
                    portPrefix.find("inproc") != string::npos
                    || portPrefix.find("ipc") != string::npos
                    || portPrefix.find("pgm") != string::npos
                    || portPrefix.find("epgm") != string::npos){
                cout << "IP address has been set to " << portPrefix << endl;
            }
            else throw(invalid_argument("Invalid ZMQ protocol provided"));
        }
        else{
            portPrefix = "tcp://127.0.0.1:";
            cout << "IP address has been set to the default value of " <<
                    portPrefix << endl;
            
        }
        if(vm.count("extra-on")){
            extraFlag = true;
            cout << "Extra port has been enabled." << endl;
        }
        if(vm.count("ccs5sub")){
            ccs5SubPort = vm["ccs5sub"].as<int>();
            if(ccs5SubPort > 65535 || ccs5SubPort < 1){
                throw(out_of_range("invalid port number"));
            }
            cout << "Port of the socket receiving data from CCS5 has been set to "
                 << ccs5SubPort << ".\n";
            ccs5SubIP = portPrefix + to_string(ccs5SubPort);
             cout << "    IP: " << ccs5SubIP << "\n";
        }
        else{
            cout << "Port of the socket receiving data from CCS5 has been set to "
                 << "its default value of 7770.\n";
            ccs5SubIP = portPrefix + "7770";
            cout << "    IP: " << ccs5SubIP << "\n";
            ccs5SubPort = 7770;
        }
        
        if(vm.count("satpub")){
            satPubPort = vm["satpub"].as<int>();
            if(satPubPort > 65535 || satPubPort < 1){
                throw(out_of_range("invalid port number"));
            }
            cout << "Port of the socket sending data to swarmFLP has been set to "
                 << satPubPort << ".\n";
            satPubIP = portPrefix + to_string(satPubPort);
             cout << "    IP: " << ccs5PubIP << "\n";
        }
        else{
            cout << "Port of the socket sending data to swarmFLP has been set to "
                 << "its default value of 7771.\n";
            satPubIP = portPrefix + "7771";
            cout << "    IP: " << satPubIP << "\n";
            satPubPort = 7771;
        }
        //
        if(vm.count("satpubheart")){
            satPubHBPort = vm["satpubheart"].as<int>();
            if(satPubHBPort > 65535 || satPubHBPort < 1){
                throw(out_of_range("invalid port number"));
            }
            cout << "Port of the heartbeat socket of RX has been set to "
                 << satPubHBPort << ".\n";
            satPubHBIP = portPrefix + to_string(satPubHBPort);
             cout << "    IP: " << satPubHBIP << "\n";
        }
        else{
            cout << "Port of the heartbeat socket of RX has been set to "
                 << "its default value of 7781.\n";
            satPubHBIP = portPrefix + "7781";
            cout << "    IP: " << satPubHBIP << "\n";
            satPubHBPort = 7781;
        }
        
        //
        if(vm.count("satsub")){
            satSubPort = vm["satsub"].as<int>();
            if(satSubPort > 65535 || satSubPort < 1){
                throw(out_of_range("invalid port number"));
            }
            cout << "Port of the socket receiving data from swarmFLP has been set to "
                 << satSubPort << ".\n";
            satSubIP = portPrefix + to_string(satSubPort);
            cout << "    IP: " << satSubIP << "\n";
        }
        else{
            cout << "Port of the socket receiving data from swarmFLP has been set to "
                 << "its default value of 7772.\n";
            satSubIP = portPrefix + "7772";
            cout << "    IP: " << satSubIP << "\n";
            satSubPort = 7772;
        }
        
        if(vm.count("satsubheart")){
            satSubHBPort = vm["satsubheart"].as<int>();
            if(satSubHBPort > 65535 || satSubHBPort < 1){
                throw(out_of_range("invalid port number"));
            }
            cout << "Port of the heartbeat socket of TX has been set to "
                 << satSubHBPort << ".\n";
            satSubHBIP = portPrefix + to_string(satSubHBPort);
            cout << "    IP: " << satSubHBIP << "\n";
        }
        else{
            cout << "Port of the heartbeat socket of TX has been set to "
                 << "its default value of 7782.\n";
            satSubHBIP = portPrefix + "7782";
            cout << "    IP: " << satSubHBIP << "\n";
            satSubHBPort = 7782;
        }
        
        if(vm.count("ccs5pub")){
            ccs5PubPort = vm["ccs5pub"].as<int>();
            if(ccs5PubPort > 65535 || ccs5PubPort < 1){
                throw(out_of_range("invalid port number"));
            }
            cout << "Port of the socket sending data to CCS5 has been set to "
                 << ccs5PubPort << ".\n";
            ccs5PubIP = portPrefix + to_string(ccs5PubPort);
            cout << "    IP: " << ccs5PubIP << "\n";
            
        }
        else{
            cout << "Port of the socket sending data to CCS5 has been set to "
                 << "its default value of 7773.\n";
            ccs5PubIP = portPrefix + "7773";
            cout << "    IP: " << ccs5PubIP << "\n";
            ccs5PubPort = 7773;
        }
        if(extraFlag){
            if(vm.count("extra")){
                extraPubPort = vm["extra"].as<int>();
                if(extraPubPort > 65535 || extraPubPort < 1){
                    throw(out_of_range("invalid port number"));
                }
                cout << "Port of the extra socket sending data has been set to "
                     << extraPubPort << ".\n";
                extraPubIP = portPrefix + to_string(extraPubPort);
            }
            else{
                cout << "Port of the extra socket sending data has been set to "
                     << "its default value of 8800.\n";

                extraPubIP = portPrefix + "8800";
                cout << "IP: " << extraPubIP << "\n";
                extraPubPort = 8800;
            }
        }
    }
    catch(const out_of_range& e){
        cerr << "error: Port number too large\n";
        return 2;
    }
    
    catch(const invalid_argument& e){
        cerr << "error: Invalid ZMQ protocol provided\n";
        cout << "Refer to the --help option." << endl;
        return 3;
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
    
    /* options */
    
    zmq::context_t context(1);
    //Part that connects to CCS5.  
    zmq::socket_t* CCS5_subscriber;
    zmq::socket_t* CCS5_publisher;
    zmq::socket_t* SAT_publisher;
    zmq::socket_t* SAT_subscriber;
    zmq::socket_t* SAT_pubHeart;
    zmq::socket_t* SAT_subHeart;
    
    //lambdas
    auto setupSockets = [&](){ //Set up the socket pointers from scratch;
                               //Sockets must be destroyed before invoking this.
        CCS5_subscriber = new zmq::socket_t(context, ZMQ_STREAM);
        CCS5_publisher = new zmq::socket_t(context, ZMQ_STREAM);

        //Part that connects to swarmFLP.
        SAT_publisher = new zmq::socket_t(context, ZMQ_PUB);  
        SAT_subscriber = new zmq::socket_t(context, ZMQ_SUB);
        SAT_subscriber->setsockopt(ZMQ_SUBSCRIBE, "", 0);

        //Part that sends empty heartbeat messages back to the satellite. 
        SAT_pubHeart = new zmq::socket_t(context, ZMQ_REQ);
        //The timeout values should be calibrated to reflect the 
        //server nature of this program. 
        SAT_pubHeart->setsockopt(ZMQ_RCVTIMEO, 500); 
        SAT_pubHeart->setsockopt(ZMQ_SNDTIMEO, -1); /**/
        SAT_subHeart = new zmq::socket_t(context, ZMQ_REP);
        SAT_subHeart->setsockopt(ZMQ_RCVTIMEO, -1); /**/ 
        SAT_subHeart->setsockopt(ZMQ_SNDTIMEO, 500); //This socket should always be 
                                                    //listening
        
        cout << "Handling binding and connecting " << endl;
        CCS5_subscriber->bind(ccs5SubIP);
        SAT_publisher->bind(satPubIP);
        SAT_pubHeart->bind(satPubHBIP);
        SAT_subHeart->bind(satSubHBIP);
    };
    auto setupHandlers = [&](){ //Init the handlers with the sockets.
        COMHandler = new msgHandler(ccs5SubPort, 
                                    satPubPort,
                                    CCS5_subscriber, 
                                    SAT_publisher,
                                    SAT_pubHeart,
                                    false); //Handles COM msg from CCS5

        PayloadHandler = new msgHandler(satSubPort,
                                        ccs5PubPort,
                                        SAT_subscriber,
                                        CCS5_publisher,
                                        SAT_subHeart,
                                        true); //Handles Payload from SIM
    };
    bool socketFlag = false;
    auto closeSockets = [&](){ //Closes the sockets. Should be run 
                                //before setupSockets.
        CCS5_subscriber->close();
        SAT_publisher->close();
        SAT_pubHeart->close();
        SAT_subHeart->close();
        socketFlag = true;
    };
    ////////////////
    
    setupSockets();
    //Part that enables additional clients to receive data. 
    // TODO: Implement this function. Maybe vectorize the handlers? 
    if(extraFlag){
        zmq::socket_t* observer_publisher = new zmq::socket_t(context, ZMQ_PUB);
    }
    
    
       
    while(1){
        
        string userinput;
        cout << "Type in a command: ";
        getline(cin, userinput);
        if(userinput == "start"){
            if(comhandlerON && pldhandlerON){
                cout << "Handlers are already working; cannot start." << endl;
            }
            else{
                if(socketFlag == true){
                    setupSockets();
                }
                comhandlerON = true;
                pldhandlerON = true;
                setupHandlers();
                comThread = thread(&msgHandler::handleMsgA, *COMHandler);
                pldThread = thread(&msgHandler::handleMsgB, *PayloadHandler);
                cout << "Started threads " << endl;
            }
        }
        else if(userinput == "terminate"){
            if(comhandlerON && pldhandlerON){
                COMHandler->setWhileFlag(false);
                PayloadHandler->setWhileFlag(false);
                if(comThread.joinable()){
                    comThread.join();
                }
                if(pldThread.joinable()){
                    pldThread.join();
                }
                delete COMHandler;
                delete PayloadHandler;
                closeSockets();
                comhandlerON = pldhandlerON = false;
            }
            else{
                cout << "Nothing to terminate!" << endl;
            }
        }
        else if(userinput == "forcerestart"){
            if(pldhandlerON && comhandlerON){
            
                COMHandler->setWhileFlag(false);
                PayloadHandler->setWhileFlag(false);
                if(comThread.joinable()){
                    comThread.join();
                }
                if(pldThread.joinable()){
                    pldThread.join();
                }
                delete COMHandler;
                delete PayloadHandler;
                closeSockets();
                setupSockets();
                setupHandlers();
                comThread = thread(&msgHandler::handleMsgA, *COMHandler);
                pldThread = thread(&msgHandler::handleMsgB, *PayloadHandler);
                cout << "Restarted threads " << endl;
                
            }
            else{
                setupSockets();
                setupHandlers();
                comThread = thread(&msgHandler::handleMsgA, *COMHandler);
                pldThread = thread(&msgHandler::handleMsgB, *PayloadHandler);
                cout << "Restarted threads " << endl;
                pldhandlerON = comhandlerON = true;
            }
            
        }
        else if(userinput == "info"){
            if(comhandlerON && pldhandlerON){
                COMHandler->printInfo();
                PayloadHandler->printInfo();
            }
            else{
                cout << "Handlers are not initialized." << endl;
            }
        }
        else if(userinput == "quit"){
            if(comhandlerON && pldhandlerON){
                COMHandler->setWhileFlag(false);
                PayloadHandler->setWhileFlag(false);
                if(comThread.joinable()){
                    comThread.join();
                }
                if(pldThread.joinable()){
                    pldThread.join();
                }
                delete COMHandler;
                delete PayloadHandler;
                closeSockets();
                comhandlerON = pldhandlerON = false;
                cout << "Goodbye" << endl;
                return 0;
                
            }
            else{
                cout << "Goodbye" << endl;
                return 0;
                
            }
        }
        else{
            cout << "Invalid command. Available commands: \n"
                 << "start          Starts the threads.\n"
                 << "terminate      Terminates all threads.\n"
                 << "forcerestart   Terminates then starts the threads.\n"
                 << "info           Prints info about the handlers.\n"
                 << "quit           Terminates the threads then returns 0."
                 << endl;
        }
    }
    return 0;
}