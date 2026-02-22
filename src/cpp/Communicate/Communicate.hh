
#ifndef NOMINMAX
#define NOMINMAX
#endif

#pragma once

// STL headers
#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <atomic>

// 3rd-party libraries
#include <open3d/Open3D.h>
#include <Eigen/Dense>
#include <OXApi/Ox.h>
#include <open62541/client.h>
#include <open62541/client_highlevel.h>

// Project headers
#include "../Device/Device.hh"
#include "../Data/Data.hh"

namespace ProfilerStreaming::Communicate
{
    /*
    This class is responsible for communicating with a device over TCP/IP.
    */
    class TCPCommunicator
    {
        public:
            TCPCommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType);
            ~TCPCommunicator();

            bool Connect();
            bool Disconnect();

            ProfilerStreaming::SpatialData::PointCloudWithTimestamp GetDataWithTimestamp();

            ProfilerStreaming::Device::DeviceType GetDeviceType() const { return this->deviceType; }

        private:
            ProfilerStreaming::Device::DeviceType deviceType;
            std::shared_ptr<Baumer::OXApi::Ox> communicationHandle = nullptr;
            std::shared_ptr<Baumer::OXApi::UdpStreaming::OxStream> streamHandle = nullptr;
    };

    class OPCUACommunicator
    {
        public:
            OPCUACommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType, std::pair<int, int> namespaceIndexAndIdentifier);
            ~OPCUACommunicator();

            bool Connect();
            bool Disconnect();

            ProfilerStreaming::SpatialData::PointCloudWithTimestamp GetDataWithTimestamp();

        private:
            std::string host;
            ProfilerStreaming::Device::DeviceType deviceType;
            std::pair<int, int> namespaceIndexAndIdentifier; // For OPC UA node identification
            UA_Client* client; // Placeholder for actual OPC UA client
    };

    class Recorder
    {
        public:
            virtual void Record(bool& recordingSwitch) = 0;
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> GetRecordedData()
            {
                return this->recordedData;
            };

        protected:
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> recordedData;
    };

    class TCPRecorder : public Recorder
    {
        public:
            TCPRecorder(TCPCommunicator& tcpCommunicator, int sleepTimeMiliSec): tcpCommunicator(tcpCommunicator), sleepTimeMiliSec(sleepTimeMiliSec) {}
            ~TCPRecorder() {}
            void Record(bool& recordingSwitch) override;

        private:
            TCPCommunicator& tcpCommunicator;
            int sleepTimeMiliSec;
    };

    class OPCUARecorder : public Recorder
    {
        public:
            OPCUARecorder(OPCUACommunicator& opcuaCommunicator, int sleepTimeMiliSec): opcuaCommunicator(opcuaCommunicator), sleepTimeMiliSec(sleepTimeMiliSec) {}
            ~OPCUARecorder() {}
            void Record(bool& recordingSwitch) override;

        private:
            OPCUACommunicator& opcuaCommunicator;
            int sleepTimeMiliSec;
    };
}