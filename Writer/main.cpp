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

#pragma warning ( push )
#pragma warning ( disable : 4457 )
#pragma warning ( pop )
#include <locale>
#include <ctime>
#include <vector>
/*
*Listens on eth0 for a given interval 
*Stores packet data in PacketStats Structure
*/
using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"
using namespace std;

int infoMode = 0;
/**
 * Mode=0 <--get requests return graph data
 * Mode=1 <--get requests return feat1
 * .
 * .
 * .
 */

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
    if(infoMode == 0){
        //TRACE(L"\nhandle GET\n");
        json::value timeSeries;
        int ctr=0;
        for (auto time : protStatsList){
            //TRACE(L"\npush time object\n");
            timeSeries["PacketStats"][ctr][time[0].first] = json::value::number(time[0].second);
            timeSeries["PacketStats"][ctr][time[1].first] = json::value::number(time[1].second);
            timeSeries["PacketStats"][ctr][time[2].first] = json::value::number(time[2].second);
            timeSeries["PacketStats"][ctr][time[3].first] = json::value::number(time[3].second);
            timeSeries["PacketStats"][ctr][time[4].first] = json::value::number(time[4].second);
            timeSeries["PacketStats"][ctr][time[5].first] = json::value::number(time[5].second);
            timeSeries["PacketStats"][ctr][time[6].first] = json::value::number(time[6].second);
            timeSeries["PacketStats"][ctr][time[7].first] = json::value::number(time[7].second);
            timeSeries["PacketStats"][ctr][time[8].first] = json::value::number(time[8].second);
            ctr++;
        }
        //cout<<timeSeries.serialize()<<endl;
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        response.set_body(timeSeries["PacketStats"]);
        request.reply(response);
    }
    else if(infoMode == 1){
        string modeResp = "{\"mode\" : 1}\n";
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        response.set_body(modeResp);
        request.reply(response);
    }
}
void handle_post(http_request request)
{
    http_response response(status_codes::OK);
    
    auto answer = json::value::object();
    request
      .extract_json()
      .then([&answer](pplx::task<json::value> task) {
         try
         {
            auto const & jvalue = task.get();
            display_json(jvalue, "R: ");

            if (!jvalue.is_null())
            {
                for (auto const & e : jvalue.as_array())
                {
                    if (e.is_integer())
                    {
                        auto key = e.as_integer();
                        infoMode = key;
                        cout<<key<<"\n";
                    }
                }
            }
         }
         catch (http_exception const & e)
         {
            wcout << e.what() << endl;
         }
      })
      .wait();
}
void *jsonServer(void *param){
    http_listener listener("http://localhost:5000");
    listener.support(methods::GET, handle_get);
    listener.support(methods::POST, handle_post);
    listener.support(methods::OPTIONS, handle_options);
    try
    {
        listener
            .open()
            .then([&listener](){TRACE(L"\nstarting to listen\n");})
            .wait();

        while (true);
    }
    catch (exception const & e)
    {
        wcout << e.what() << endl;
    }
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

	pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr);
	if (dev == NULL)
	{
		std::cerr << "Cannot find interface with IPv4 address of '" << interfaceIPAddr << "'" << std::endl;
		return 1;
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
		return 1;
	}
    /*Pcap device thread*/
	pcpp::RawPacketVector packetVec;
	// start capturing packets. All packets will be added to the packet vector
    
    int initialStats = 1;
    int time = 1000;
    TypeCounts initialTypeStats = statsExec.getPacketTypeCounts();
    for(int i=0; i<initialStats; i++){
        vector<pair<string, int>> protStats;
        protStats.push_back(make_pair("time", time));
        time+=1000;
        protStats.push_back(make_pair("ETH", initialTypeStats.ethPacketCount));
        protStats.push_back(make_pair("IPV4", initialTypeStats.ipv4PacketCount));
        protStats.push_back(make_pair("IPV6", initialTypeStats.ipv6PacketCount));
        protStats.push_back(make_pair("UDP", initialTypeStats.udpPacketCount));
        protStats.push_back(make_pair("TCP", initialTypeStats.tcpPacketCount));
        protStats.push_back(make_pair("DNS", initialTypeStats. dnsPacketCount));
        protStats.push_back(make_pair("HTTP", initialTypeStats.httpPacketCount));
        protStats.push_back(make_pair("SSL", initialTypeStats.sslPacketCount));
        protStatsList.push_back(protStats);
    }
	dev->startCapture(packetVec);
    while(1){
        sleep(5);
        for (pcpp::RawPacketVector::ConstVectorIterator iter = packetVec.begin(); iter != packetVec.end(); iter++)
        {
            // parse raw packet
            pcpp::Packet parsedPacket(*iter);

            // feed packet to the stats object
            statsExec.consumePacket(parsedPacket);
        }
        vector<pair<string, int>> protStats;
        TypeCounts typeStats = statsExec.getPacketTypeCounts();
        protStats.push_back(make_pair("time", time));
        time+=1000;
        protStats.push_back(make_pair("ETH", typeStats.ethPacketCount));
        protStats.push_back(make_pair("IPV4", typeStats.ipv4PacketCount));
        protStats.push_back(make_pair("IPV6", typeStats.ipv6PacketCount));
        protStats.push_back(make_pair("UDP", typeStats.udpPacketCount));
        protStats.push_back(make_pair("TCP", typeStats.tcpPacketCount));
        protStats.push_back(make_pair("DNS", typeStats.dnsPacketCount));
        protStats.push_back(make_pair("HTTP", typeStats.httpPacketCount));
        protStats.push_back(make_pair("SSL", typeStats.sslPacketCount));

        protStatsList.push_back(protStats);
        if(protStatsList.size() > 6)
        {
            protStatsList.erase(protStatsList.begin());
        }
    }

    
	// stop capturing packets
	dev->stopCapture();
}
