#include <iostream>
#include <unistd.h>
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include <stdio.h>
#include <string.h> /* For strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string>
#include <fstream>

#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/http_msg.h>                   // JSON Utility Functions
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/asyncrt_utils.h>              // Async helpers
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios
#include <cpprest/filestream.h>

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>

#include "StatsExecutive.h"

#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "PcapFileDevice.h"

#include <locale>
#include <ctime>
#include <vector>
#include <iostream>
#include <list>
#include <cstdlib>
#include <cassert>
#define PCAP_FILE "simData.pcap"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace std;

pcpp::Packet * glbPacket = nullptr; // packet being served



long unsigned int globId = 0;
long unsigned int pktCtr = 0;

pcpp::IFileReaderDevice* reader ; //global reader
std::list<pcpp::Packet*> liveList;  
pcpp::Packet * getSim(){
    if(reader != nullptr){
        if (!reader->open())
        {
            return nullptr;
        }else{
            pcpp::RawPacket rawPacket;
            reader->getNextPacket(rawPacket);
            globId++;
            return new pcpp::Packet(&rawPacket);
        }
    }
}
std::string getListenIp()
{
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "lo", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    std::string ip_addr = std::string(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    std::cout<<"interface ip: "<<ip_addr<<std::endl;
    return ip_addr;
}
void display_json(
   json::value const & jvalue,
   utility::string_t const & prefix)
{
   cout << prefix << jvalue.serialize() << endl;
}
void handle_options(http_request request)
{
    http_response response(status_codes::OK);
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    request.reply(response);
}

vector<vector<pair<string, int>>> protStatsList;
void handle_get(http_request request)
{
    http_response response(status_codes::OK);
    
    /* create json packet structure */
    json::value retPacket;
    pcpp::Layer* pitr = nullptr;
    pcpp::Packet * pkt = nullptr;
    int itr = 0;
    
    if(request.headers().has("Content-Type")){//simulated
        //return multiple simulated data packets
        std::list<pcpp::Packet*> simList;
        do{
            pkt = getSim();
            simList.push_back(pkt);
        } while( (rand() % 10) < 5 );
        assert(simList.front() != nullptr);
        retPacket["id"] = globId;//return ctr of num sim requests
        while(simList.size() > 0){//live list case
            pitr = simList.front()->getFirstLayer();
            while(pitr != nullptr){
                
                retPacket["packetList"][itr]["packet"][std::to_string(pitr->getOsiModelLayer())] = 
                    json::value::string(pitr->toString());
                // if(pitr->getProtocol() == pcpp::GenericPayload){
                //     std::cout << (char *)(pitr->getLayerPayload()) << "\n";
                // }
                pitr = pitr->getNextLayer();
            }
            assert(simList.front() != nullptr); // simList should never hold null
            simList.pop_front();
            itr++;
        }
    }else{   
         
        retPacket["id"] = pktCtr;
        while(liveList.size() > 0){//live list case
            pitr = liveList.front()->getFirstLayer();
            while(pitr != nullptr){
                retPacket["packetList"][itr]["packet"][std::to_string(pitr->getOsiModelLayer())] = 
                    json::value::string(pitr->toString());
                pitr = pitr->getNextLayer();
            }
            if(liveList.front() != nullptr){
                delete liveList.front();
            }
            liveList.pop_front();
            itr++;
        }
    }

    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    // display_json( retPacket, json::value::string(""));
    response.set_body(retPacket);
    request.reply(response);
    //https://stackoverflow.com/questions/44597525/cpp-rest-sdk-json-how-to-create-json-w-array-and-write-to-file
}
void *jsonServer(void *param){
    http_listener listener("http://localhost:5000");
    listener.support(methods::GET, handle_get);
    listener.support(methods::OPTIONS, handle_options);
    try
    {
        listener
            .open()
            .then([&listener](){wcout << (L"\nstarting to listen\n");})
            .wait();

        while (true);
    }
    catch (exception const & e)
    {
        wcout << e.what() << endl;
    }
}

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{

    // if (glbPacket != nullptr){ // delete last packet -- 
    //     delete glbPacket; 
    // }
    // pktCtr++;
    // glbPacket = new pcpp::Packet(packet); //new packet out of raw packet
    liveList.push_back(new pcpp::Packet(packet));
    pktCtr++;
}

bool devValid(pcpp::PcapLiveDevice* dev, std::string interfaceIPAddr){
    if (dev == NULL)
	{
		std::cerr << "Cannot find interface with IPv4 address of '" << interfaceIPAddr << "'" << std::endl;
		return false;
	}
	std::cout
    << "Interface info:" << std::endl
    << "   Interface name:        " << dev->getName() << std::endl // get interface name
    << "   Interface description: " << dev->getDesc() << std::endl // get interface description
    << "   MAC address:           " << dev->getMacAddress() << std::endl // get interface MAC address
    << "   Default gateway:       " << dev->getDefaultGateway() << std::endl // get default gateway
    << "   Interface MTU:         " << dev->getMtu() << std::endl; // get interface MTU

	if (dev->getDnsServers().size() > 0)
		std::cout << "   DNS server:            " << dev->getDnsServers().at(0) << std::endl;
	if (!dev->open())
	{
		std::cerr << "Cannot open device" << std::endl;
		return false;
	}
    return true;
}

int main(int argc, char* argv[])
{
    /* create thread for json server */
    pthread_t serv;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&serv, &attr, jsonServer, (void *)NULL);
    
    /*front end request listener*/
    std::string interfaceIPAddr = getListenIp();

    /* create stats executive object */
    StatsExecutive statsExec;

    /*global pcap reader opens file */
    reader = pcpp::IFileReaderDevice::getReader(PCAP_FILE);

	pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr);
	if(!devValid(dev, interfaceIPAddr)){
        return 1;
    }
    
	dev->startCapture(onPacketArrives, nullptr);
    while(1){
        sleep(5);
    }
	// stop capturing packets
	dev->stopCapture();
}
