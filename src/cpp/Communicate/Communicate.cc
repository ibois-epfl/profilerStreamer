
#include "Communicate.hh"

namespace ProfilerStreaming::Communicate
{
    TCPCommunicator::TCPCommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType) : deviceType(deviceType)
    {
        if (deviceType == ProfilerStreaming::Device::DeviceType::OX)
        {
            this->communicationHandle = Baumer::OXApi::Ox::Create(host);
        }
    }

    TCPCommunicator::~TCPCommunicator()
    {
        if (this->deviceType == ProfilerStreaming::Device::DeviceType::OX)
        {
            if (this->communicationHandle)
                this->communicationHandle->Disconnect();
            
            if (this->streamHandle)
                this->streamHandle->Stop();
                this->streamHandle->Close();
            this->streamHandle = nullptr;
            this->communicationHandle = nullptr;
        }
    }

    bool TCPCommunicator::Connect()
    {
        if (this->deviceType == ProfilerStreaming::Device::DeviceType::OX)
        {
            if (this->communicationHandle)
                this->communicationHandle->Connect();
            
                if (!this->streamHandle)
            {
                this->streamHandle = this->communicationHandle->CreateStream();
                this->streamHandle->Start();
            }
            
        }
        else
        {
            throw std::invalid_argument("Unsupported device type");
        }
        return true; // Placeholder return value
    }

    bool TCPCommunicator::Disconnect()
    {
        if (this->deviceType == ProfilerStreaming::Device::DeviceType::OX)
        {
            if (this->communicationHandle)
                this->communicationHandle->Disconnect();

            if (this->streamHandle)
            {
                this->streamHandle->Stop();
                this->streamHandle->Close();
            }
            this->communicationHandle = nullptr;
        }
        return true; // Placeholder return value
    }

    ProfilerStreaming::SpatialData::PointCloudWithTimestamp TCPCommunicator::GetDataWithTimestamp()
    {
        if (this->deviceType == ProfilerStreaming::Device::DeviceType::OX)
        {
            if (!this->communicationHandle)
            {
                throw std::runtime_error("Communication handle is not initialized");
            }
            if (!this->streamHandle)
            {
                this->streamHandle = this->communicationHandle->CreateStream();
                // this->streamHandle->SetReceiveBufferSize( 2 * 1024 * 1024 );
                this->streamHandle->Start();
            }
            std::vector<Eigen::Vector3d> points;
            auto timestamp = std::chrono::system_clock::now();
            if( this->streamHandle->ProfileAvailable( ) )
            {
                Baumer::OXApi::UdpStreaming::ProfilePacket profile = this->streamHandle->ReadProfile();
                const Baumer::OXApi::Types::Profile profileInfo = this->communicationHandle->GetProfile();
                auto timestamp = std::chrono::system_clock::now();
                
                for (u_int i = 0; i < profile.Length; ++i)
                {
                    if (profile.X.at(i) == 0 && profile.Z.at(i) == 0)
                        continue; // skip invalid points
                    else if (profile.X.at(i) && profile.Z.at(i))
                    {
                        double y = (profile.X.at(i) + profileInfo.XStart) / (double)profileInfo.Precision;
                        double z = (profile.Z.at(i)) / (double)profileInfo.Precision;
                        points.emplace_back(0, y, z); // Assuming the global coord system has y in the profiler's x direction.
                    }
                }
            }
            this->streamHandle->ClearProfileQueue();
            this->streamHandle->ClearMeasurementQueue();
            return ProfilerStreaming::SpatialData::PointCloudWithTimestamp(points, timestamp);
        }
        else
        {
            throw std::invalid_argument("Unsupported device type");
        }
    }

    OPCUACommunicator::OPCUACommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType, std::pair<int, int> namespaceIndexAndIdentifier) : host(host), deviceType(deviceType), namespaceIndexAndIdentifier(namespaceIndexAndIdentifier)
    {
        if (deviceType != ProfilerStreaming::Device::DeviceType::IO_LINK)
        {
            throw std::invalid_argument("Unsupported device type for OPC UA communication");
        }
        UA_Client *client = UA_Client_new();
        this->client = client;
    }

    OPCUACommunicator::~OPCUACommunicator()
    {
        if (this->client)
        {
            UA_Client_delete(this->client);
        }
    }

    bool OPCUACommunicator::Connect()
    {
        if (this->client)
        {
            UA_StatusCode status = UA_Client_connect(this->client, this->host.c_str());
            if(status != UA_STATUSCODE_GOOD) 
            {
                UA_Client_delete(this->client);
                this->client = nullptr;
                throw std::runtime_error("Failed to connect to OPC UA server");
                return false;
            }
        }
        return true;
    }

    bool OPCUACommunicator::Disconnect()
    {
        if (this->client)
        {
            UA_Client_disconnect(this->client);
            UA_Client_delete(this->client);
            this->client = nullptr;
        }
        return true;
    }

    ProfilerStreaming::SpatialData::PointCloudWithTimestamp OPCUACommunicator::GetDataWithTimestamp()
    {
        if (!this->client)
        {
            throw std::runtime_error("OPC UA client is not connected");
        }

        UA_Variant value;
        UA_Variant_init(&value);
        UA_NodeId nodeId = UA_NODEID_NUMERIC(this->namespaceIndexAndIdentifier.first, this->namespaceIndexAndIdentifier.second);
        UA_StatusCode status = UA_Client_readValueAttribute(this->client, nodeId, &value);
        if(status != UA_STATUSCODE_GOOD) 
        {
            throw std::runtime_error("Failed to read value from OPC UA server");
        }

        std::vector<Eigen::Vector3d> points;
        if(UA_Variant_hasArrayType(&value, &UA_TYPES[UA_TYPES_BYTE])) 
        {
            UA_Byte* data = (UA_Byte*)value.data;
            size_t len = value.arrayLength;

            if (len != 2)
            {
                throw std::runtime_error("Expected a byte array of length 2 for distance measurement");
            }
            else
            {
                uint16_t value_mm = (data[0] << 8) | data[1];
                Eigen::Vector3d point(value_mm, 0, 0);
                points.push_back(point);
            }
        }
        else
        {
            throw std::runtime_error("Unexpected data type received from OPC UA server");
        }

        auto timestamp = std::chrono::system_clock::now();
        return ProfilerStreaming::SpatialData::PointCloudWithTimestamp(points, timestamp);
    }

    TCPRecorder::~TCPRecorder()
    {
        this->StopRecording();
    }


    void TCPRecorder::StartRecording()
    {
        this->recordingSwitch = true;

        this->recordingThread = std::thread([this]()
        {
            while (this->recordingSwitch)
            {
                auto profile = this->tcpCommunicator.GetDataWithTimestamp();
                this->recordedData.push_back(profile);
                std::this_thread::sleep_for(std::chrono::milliseconds(this->sleepTimeMiliSec));
            }
        });
        this->recordingThread.detach();
    }

    void TCPRecorder::StopRecording()
    {
        this->recordingSwitch = false;
    }

    OPCUARecorder::~OPCUARecorder()
    {
        this->StopRecording();
    }

    void OPCUARecorder::StartRecording()
    {
        this->recordingSwitch = true;
        this->recordingThread = std::thread([this]()
        {
            while (this->recordingSwitch)
            {
                auto data = this->opcuaCommunicator.GetDataWithTimestamp();
                this->recordedData.push_back(data);
                std::this_thread::sleep_for(std::chrono::milliseconds(this->sleepTimeMiliSec));
            }
        });
        this->recordingThread.detach();
    }

    void OPCUARecorder::StopRecording()
    {
        this->recordingSwitch = false;
    }

}