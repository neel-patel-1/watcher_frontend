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
/*
*Listens on eth0 for a given interval 
*writes traffic stats to db.json file
*/

struct PacketStats
{
    int ethPacketCount;
    int ipv4PacketCount;
    int ipv6PacketCount;
    int tcpPacketCount;
    int udpPacketCount;
    int dnsPacketCount;
    int httpPacketCount;
    int sslPacketCount;

    /**
    * Clear all stats
    */
    void clear() { ethPacketCount = 0; ipv4PacketCount = 0; ipv6PacketCount = 0; tcpPacketCount = 0; udpPacketCount = 0; tcpPacketCount = 0; dnsPacketCount = 0; httpPacketCount = 0; sslPacketCount = 0; }

    /**
    * C'tor
    */
    PacketStats() { clear(); }

    /**
    * Collect stats from a packet
    */
    void consumePacket(pcpp::Packet& packet)
    {
        if (packet.isPacketOfType(pcpp::Ethernet))
            ethPacketCount++;
        if (packet.isPacketOfType(pcpp::IPv4))
            ipv4PacketCount++;
        if (packet.isPacketOfType(pcpp::IPv6))
            ipv6PacketCount++;
        if (packet.isPacketOfType(pcpp::TCP))
            tcpPacketCount++;
        if (packet.isPacketOfType(pcpp::UDP))
            udpPacketCount++;
        if (packet.isPacketOfType(pcpp::DNS))
            dnsPacketCount++;
        if (packet.isPacketOfType(pcpp::HTTP))
            httpPacketCount++;
        if (packet.isPacketOfType(pcpp::SSL))
            sslPacketCount++;
    }

    /**
    * Print stats to console
    */
    void printToConsole()
    {
        std::cout
            << "Ethernet packet count: " << ethPacketCount << std::endl
            << "IPv4 packet count:     " << ipv4PacketCount << std::endl
            << "IPv6 packet count:     " << ipv6PacketCount << std::endl
            << "TCP packet count:      " << tcpPacketCount << std::endl
            << "UDP packet count:      " << udpPacketCount << std::endl
            << "DNS packet count:      " << dnsPacketCount << std::endl
            << "HTTP packet count:     " << httpPacketCount << std::endl
            << "SSL packet count:      " << sslPacketCount << std::endl;
    }
};
std::string getListenIp()
{
    int fd;
    struct ifreq ifr;
	    fd = socket(AF_INET, SOCK_DGRAM, 0);
        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
        ioctl(fd, SIOCGIFADDR, &ifr);
        close(fd);
        std::string ip_addr = std::string(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
        std::cout<<"interface ip: "<<ip_addr<<std::endl;
        return ip_addr;
}
int main(int argc, char* argv[])
{
    std::string interfaceIPAddr = getListenIp();
	PacketStats stats;
    int time = 2000;
    

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

	pcpp::RawPacketVector packetVec;

	// start capturing packets. All packets will be added to the packet vector
    std::string newDBLine;
    std::string prev;

    std::string kLines;

	dev->startCapture(packetVec);
    while(1){
        sleep(5);
        for (pcpp::RawPacketVector::ConstVectorIterator iter = packetVec.begin(); iter != packetVec.end(); iter++)
        {
            // parse raw packet
            pcpp::Packet parsedPacket(*iter);

            // feed packet to the stats object
            stats.consumePacket(parsedPacket);
        }
        int ctr = 0;
        std::ifstream oDB("../db.json");
        std::ofstream nDB("../nDB.json");
        //for testing
        newDBLine = "\t\t{ \"time\": " + std::to_string(time+=1000) + 
            ", \"ETH\": " + std::to_string(stats.ethPacketCount) +
            ", \"IPV4\": " + std::to_string(stats.ipv4PacketCount) +
            ", \"IPV6\": " + std::to_string(stats.ipv6PacketCount) +
            ", \"UDP\": " + std::to_string(stats.udpPacketCount) + 
            ", \"TCP\": " + std::to_string(stats.tcpPacketCount) + 
            ", \"DNS\": " + std::to_string(stats.dnsPacketCount) + 
            ", \"HTTP\": " + std::to_string(stats.httpPacketCount) + 
            ", \"SSL\": " + std::to_string(stats.sslPacketCount) + 
            "}";
        while(getline(oDB, kLines)){
            std::cout<<"got: "<<kLines<<"\n";
            // if(kLines.find_first_of('{') != std::string::npos && kLines.length() > 1){
            // }
            size_t curlPos = kLines.find('}');

            if( curlPos != std::string::npos && kLines.find_last_of(',') < curlPos )
            {
                kLines.append(",");
            }
            if(kLines.find_first_of(']')!=std::string::npos){
                nDB<<newDBLine<<"\n";
                std::cout<<"appended : " << newDBLine << "\n";
            }
            nDB<<kLines<<"\n";
            ctr++;
            
            
        }

        oDB.close();
        nDB.close();
        if(ctr > 20 ){//clear both
            std::fstream clearBoth;
            clearBoth.open("../db.json", std::ios::out | std::ios::trunc);
            clearBoth.close();
            clearBoth.open("../nDB.json", std::ios::out | std::ios::trunc);
            clearBoth<<"{\n"
                <<"  \"PacketStats\": [\n"
                <<prev<<",\n"
                <<newDBLine<<"\n"
                <<"	  ]\n"
                <<"}";
            clearBoth.close();
        }
        prev = newDBLine;
        ctr=0;
        std::remove("../db.json");
        std::rename("../nDB.json", "../db.json");


        // append stats to db.json
        //{ "time": 2000, "UDP": 2, "TCP": 2, "ICMP": 2} 
        
    }
	
    
    

	// stop capturing packets
	dev->stopCapture();
	stats.printToConsole();
}
