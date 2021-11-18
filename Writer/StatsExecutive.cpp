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
}

TypeCounts StatsExecutive::getPacketTypeCounts() const 
{
    return typeCounts;
}