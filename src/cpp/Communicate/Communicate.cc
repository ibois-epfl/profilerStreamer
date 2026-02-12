
#include "Communicate.hh"

namespace ProfilerStreaming::Communicate
{
    TCPCommunicator::TCPCommunicator(const std::string& host, DeviceType deviceType) : deviceType(deviceType)
    {
        if (deviceType == DeviceType::OX)
        {
            try
            {
                this->communicationHandle = Baumer::OXApi::Ox::Create(host);
            }
            catch (const std::exception& e)
            {
                throw std::runtime_error("Failed to initialize OX device communication: " + std::string(e.what()));
            }
        }
        else
        {
            throw std::invalid_argument("Unsupported device type");
        }
    }

    TCPCommunicator::~TCPCommunicator()
    {
        if (this->deviceType == DeviceType::OX)
        {
            if (this->communicationHandle)
                this->communicationHandle->Disconnect();
        }
        else
        {
            throw std::invalid_argument("Unsupported device type");
        }
    }

    bool TCPCommunicator::Connect()
    {
        if (this->deviceType == DeviceType::OX)
        {
            if (this->communicationHandle)
                this->communicationHandle->Connect();
        }
        else
        {
            throw std::invalid_argument("Unsupported device type");
        }
        return true; // Placeholder return value
    }

    bool TCPCommunicator::Disconnect()
    {
        if (this->deviceType == DeviceType::OX)
        {
            if (this->communicationHandle)
                this->communicationHandle->Disconnect();
        }
        else
        {
            throw std::invalid_argument("Unsupported device type");
        }
        return true; // Placeholder return value
    }

    std::pair<std::vector<Eigen::Vector3d>, std::chrono::time_point<std::chrono::system_clock>> TCPCommunicator::GetDataWithTimestamp()
    {
        if (this->deviceType == DeviceType::OX)
        {
            if (!this->communicationHandle)
                throw std::runtime_error("Communication handle is not initialized");
            Baumer::OXApi::Types::Profile profile = this->communicationHandle->GetProfile();
            auto timestamp = std::chrono::system_clock::now();
            std::vector<Eigen::Vector3d> points;
            for (u_int i = 0; i < profile.Length; ++i)
            {
                if (profile.X.at(i) == 0 && profile.Z.at(i) == 0)
                    continue; // skip invalid points
                else if (profile.X.at(i) && profile.Z.at(i))
                {
                    double x = (profile.X.at(i) + profile.XStart) / (double)profile.Precision;
                    double z = (profile.Z.at(i)) / (double)profile.Precision;
                    points.emplace_back(x, 0, z); // Assuming Y is 0 for 2D profiles
                }
            }
            return {points, timestamp};
        }
        else
        {
            throw std::invalid_argument("Unsupported device type");
        }
    }

    OPCUACommunicator::OPCUACommunicator(const std::string& host, DeviceType deviceType, std::pair<int, int> namespaceIndexAndIdentifier) : host(host), deviceType(deviceType), namespaceIndexAndIdentifier(namespaceIndexAndIdentifier)
    {
        if (deviceType != DeviceType::IO_LINK)
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

    std::pair<std::vector<Eigen::Vector3d>, std::chrono::time_point<std::chrono::system_clock>> OPCUACommunicator::GetDataWithTimestamp()
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
        return {points, timestamp};
    }

}