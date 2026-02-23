
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
            /*
            Constructor for the TCPCommunicator class.
            @param host The IP address of the device to connect to.
            @param deviceType The type of device to communicate with. Currently, only the OX device type is supported.
            */
            TCPCommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType);
            ~TCPCommunicator();

            /*
            Establishes a connection to the device.
            @return True if the connection was successful, false otherwise.
            */
            bool Connect();

            /*
            Closes the connection to the device.
            @return True if the disconnection was successful, false otherwise.
            */
            bool Disconnect();

            /*
            Retrieves data from the device along with a timestamp.
            @return A PointCloudWithTimestamp object containing the data and timestamp.
            */
            ProfilerStreaming::SpatialData::PointCloudWithTimestamp GetDataWithTimestamp();

            /*
            Getter for the device type.
            @return The device type of this communicator.
            */
            ProfilerStreaming::Device::DeviceType GetDeviceType() const { return this->deviceType; }

        private:
            /*
            The type of device this communicator is associated with, as private member.
            */
            ProfilerStreaming::Device::DeviceType deviceType;

            /*
            The communication handle for the device, as a private member. This is used to manage the connection and communication with the device.
            */
            std::shared_ptr<Baumer::OXApi::Ox> communicationHandle = nullptr;
            std::shared_ptr<Baumer::OXApi::UdpStreaming::OxStream> streamHandle = nullptr;
    };

    class OPCUACommunicator
    {
        public:
            /*
            Constructor for the OPCUACommunicator class.
            @param host The IP address of the OPC UA server to connect to.
            @param deviceType The type of device to communicate with.
            @param namespaceIndexAndIdentifier A pair representing the namespace index and identifier for the OPC UA node.
            */
            OPCUACommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType, std::pair<int, int> namespaceIndexAndIdentifier);
            ~OPCUACommunicator();

            /*
            Establishes a connection to the OPC UA server.
            @return True if the connection was successful, false otherwise.
            */
            bool Connect();

            /*
            Closes the connection to the OPC UA server.
            @return True if the disconnection was successful, false otherwise.
            */
            bool Disconnect();

            /*
            Retrieves data from the OPC UA server along with a timestamp.
            @return A PointCloudWithTimestamp object containing the data and timestamp.
            */
            ProfilerStreaming::SpatialData::PointCloudWithTimestamp GetDataWithTimestamp();

        private:
            /*
            The IP address of the OPC UA server, as a private member.
            */
            std::string host;

            /*
            The type of device this communicator is associated with, as a private member.
            */
            ProfilerStreaming::Device::DeviceType deviceType;

            /*
            A pair representing the namespace index and identifier for the OPC UA node, as a private member.
            */
            std::pair<int, int> namespaceIndexAndIdentifier; // For OPC UA node identification
            /*
            The OPC UA client, as a private member. This is used to manage the connection and communication with the OPC UA server.
            */
            UA_Client* client; // Placeholder for actual OPC UA client
    };

    /*
    A recorder class that can be used to record data from a communicator. It uses a separate thread to continuously record data while a boolean switch is true.
    */
    class Recorder
    {
        public:
            Recorder(int sleepTimeMiliSec) : sleepTimeMiliSec(sleepTimeMiliSec) {}
            virtual ~Recorder() {}

            virtual void StartRecording() = 0;

            virtual void StopRecording() = 0;

            /*
            Retrieves the recorded data.
            @return A vector of PointCloudWithTimestamp objects containing the recorded data.
            */
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> GetRecordedData()
            {
                return this->recordedData;
            };

        protected:
            /*
            A vector to store the recorded data, as a protected member. This allows derived classes to access and modify the recorded data.
            */
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> recordedData;

            /*
            The time in milliseconds to wait between recording data points, as a protected member.
            */
            int sleepTimeMiliSec;

            /*
            A thread used for recording data in the background, as a protected member.
            */
            std::thread recordingThread;

            std::atomic<bool> recordingSwitch;
    };

    class TCPRecorder : public Recorder
    {
        public:
            /*
            Constructor for the TCPRecorder class.
            @param tcpCommunicator A reference to a TCPCommunicator object to use for recording data.
            @param sleepTimeMiliSec The time in milliseconds to wait between recording data points.
            */
            TCPRecorder(TCPCommunicator& tcpCommunicator, int sleepTimeMiliSec): Recorder(sleepTimeMiliSec), tcpCommunicator(tcpCommunicator) {}
            ~TCPRecorder();

            /*
            Records data from the TCPCommunicator in a separate thread while the recording switch is true.
            @param recordingSwitch A reference to an atomic boolean that controls the recording loop.
            */
            void StartRecording() override;

            void StopRecording() override;

        private:
            /*
            A reference to the TCPCommunicator object used for recording data, as a private member.
            */
            TCPCommunicator& tcpCommunicator;
    };

    class OPCUARecorder : public Recorder
    {
        public:
            /*
            Constructor for the OPCUARecorder class.
            @param opcuaCommunicator A reference to an OPCUACommunicator object to use for recording data.
            @param sleepTimeMiliSec The time in milliseconds to wait between recording data points.
            */
            OPCUARecorder(OPCUACommunicator& opcuaCommunicator, int sleepTimeMiliSec): Recorder(sleepTimeMiliSec), opcuaCommunicator(opcuaCommunicator) {}
            ~OPCUARecorder();
            /*
            Records data from the OPCUACommunicator in a separate thread while the recording switch is true.
            @param recordingSwitch A reference to an atomic boolean that controls the recording loop.
            */
            void StartRecording() override;

            void StopRecording() override;

        private:
            /*
            A reference to the OPCUACommunicator object used for recording data, as a private member.
            */
            OPCUACommunicator& opcuaCommunicator;
    };
}