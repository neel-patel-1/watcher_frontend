#include <map>
#include <IpAddress.h>
#include <IPLayer.h>
#include "stdlib.h"
#include "PcapLiveDeviceList.h"

struct HostGraph {
    /* map of traffic between two hosts */
    /* first index is source, second index is dest */
    std::map<std::string, std::vector<pcpp::Packet>> srcMap;
    std::map<std::string, std::vector<pcpp::Packet>> dstMap;

    void clear() {
        srcMap = {};
        dstMap = {};
    }

    HostGraph() {
        clear();
    }
};

struct TypeCounts
{
    int ethPacketCount;
    int ipv4PacketCount;
    int ipv6PacketCount;
    int tcpPacketCount;
    int udpPacketCount;
    int dnsPacketCount;
    int httpPacketCount;
    int sslPacketCount;
    void clear() { 
        ethPacketCount = 0; 
        ipv4PacketCount = 0; 
        ipv6PacketCount = 0; 
        tcpPacketCount = 0; 
        udpPacketCount = 0; 
        tcpPacketCount = 0; 
        dnsPacketCount = 0; 
        httpPacketCount = 0; 
        sslPacketCount = 0; 
    }
    TypeCounts() { 
        clear(); 
    }
};


#ifndef STATS_EXECUTIVE_H
#define STATS_EXECUTIVE_H

class StatsExecutive 
{
private: 
    TypeCounts typeCounts;
    HostGraph hostData;

    void processHosts(pcpp::Packet& packet);

public:
    StatsExecutive();
    ~StatsExecutive();
    void consumePacket(pcpp::Packet& packet);
    TypeCounts getPacketTypeCounts() const;

};

#endif
