
#ifndef NOMINMAX
#define NOMINMAX
#endif

#pragma once

// STL headers
#include <vector>
#include <string>
#include <utility>
#include <chrono>

// 3rd-party libraries
#include <open3d/Open3D.h>
#include <Eigen/Dense>
#include <OXApi/Ox.h>
#include <open62541/client.h>
#include <open62541/client_highlevel.h>

// Project headers
#include "../Device/Device.hh"

namespace ProfilerStreaming::Communicate
{
    /*
    This class is responsible for communicating with a device over TCP/IP.
    */
    class TCPCommunicator
    {
        public:
            TCPCommunicator(const std::string& host, DeviceType deviceType);
            ~TCPCommunicator();

            bool Connect();
            bool Disconnect();

            std::pair<std::vector<Eigen::Vector3d>, std::chrono::time_point<std::chrono::system_clock>> GetDataWithTimestamp();

            DeviceType GetDeviceType() const { return this->deviceType; }

        private:
            DeviceType deviceType;
            std::shared_ptr<Baumer::OXApi::Ox> communicationHandle = nullptr;
    };

    class OPCUACommunicator
    {
        public:
            OPCUACommunicator(const std::string& host, DeviceType deviceType, std::pair<int, int> namespaceIndexAndIdentifier);
            ~OPCUACommunicator();

            bool Connect();
            bool Disconnect();

            std::pair<std::vector<Eigen::Vector3d>, std::chrono::time_point<std::chrono::system_clock>> GetDataWithTimestamp();

        private:
            std::string host;
            DeviceType deviceType;
            std::pair<int, int> namespaceIndexAndIdentifier; // For OPC UA node identification
            UA_Client* client; // Placeholder for actual OPC UA client
    };
}