/*
 * OXApi/Types/Interface.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_INTERFACE_H_
#define OXAPI_TYPES_INTERFACE_H_

#include <string>
#include <vector>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct NetworkConfiguration {
    bool DhcpActive;
    std::string IpAddress;
    std::string SubnetMask;
    std::string Gateway;
    std::string MacAddress;
};

struct RealtimeProtocol {
    const std::uint32_t Id;
    const std::string Name;
};

struct IoLinkProcessDataLayout {
    const std::uint32_t Id;
    const std::string Name;
};

struct ProcessInterfacesInfo {
    const std::vector<RealtimeProtocol> RealtimeProtocols;
    const std::vector<IoLinkProcessDataLayout> IoLinkProcessDataLayouts;
};

struct ProcessInterfaces {
    bool ModbusEnabled;
    bool OPCUAEnabled;
    std::uint32_t RealtimeProtocol;
    bool UdpStreamingEnabled;
    std::string UdpStreamingIp;
    std::uint32_t UdpStreamingPort;
    std::uint32_t IoLinkProcessDataLayout;
};

struct TimeServerInfo {
    bool Enabled;
    std::vector<std::string> TimeServers;
};

struct UdpStream {
    const std::uint32_t Id;
    const std::string Name;
};

struct UdpStreamInfo {
    const std::vector<UdpStream> UdpStreams;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_INTERFACE_H_ */
