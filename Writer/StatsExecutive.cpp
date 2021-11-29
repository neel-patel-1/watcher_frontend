#include <iostream>

#include "StatsExecutive.h"
using namespace std;

StatsExecutive::StatsExecutive() 
{}

StatsExecutive::~StatsExecutive() 
{}

void StatsExecutive::consumePacket(pcpp::Packet& packet) 
{
    if (packet.isPacketOfType(pcpp::Ethernet))
        typeCounts.ethPacketCount++;
    if (packet.isPacketOfType(pcpp::IPv4))
        typeCounts.ipv4PacketCount++;
    if (packet.isPacketOfType(pcpp::IPv6))
        typeCounts.ipv6PacketCount++;
    if (packet.isPacketOfType(pcpp::TCP))
        typeCounts.tcpPacketCount++;
    if (packet.isPacketOfType(pcpp::UDP))
        typeCounts.udpPacketCount++;
    if (packet.isPacketOfType(pcpp::DNS))
        typeCounts.dnsPacketCount++;
    if (packet.isPacketOfType(pcpp::HTTP))
        typeCounts.httpPacketCount++;
    if (packet.isPacketOfType(pcpp::SSL))
        typeCounts.sslPacketCount++;

    this->processHosts(packet);
}

void StatsExecutive::processHosts(pcpp::Packet& packet) 
{
    pcpp::IPLayer* ipLayer = packet.getLayerOfType<pcpp::IPLayer>();
    std::string source = ipLayer->getSrcIPAddress().toString();
    std::string dest = ipLayer->getDstIPAddress().toString();

    std::cout << "source: " << source 
    << ", dest: " << dest << std::endl;

    // find will return IP addr (ref'd by ->first)

    // add to srcMap
    auto srcSearch = hostData.srcMap.find(source);
    if (srcSearch != hostData.srcMap.end()) {
        hostData.srcMap[source].push_back(packet);
    } else {
        std::vector<pcpp::Packet> v = { packet };
        hostData.srcMap.insert({source, v});
    }

    // add to dstMap
    auto dstSearch = hostData.dstMap.find(dest);
    if (dstSearch != hostData.dstMap.end()) {
        hostData.dstMap[dest].push_back(packet);
    } else {
        std::vector<pcpp::Packet> v = { packet };
        hostData.dstMap.insert({source, v});
    }
}

TypeCounts StatsExecutive::getPacketTypeCounts() const 
{
    return typeCounts;
}