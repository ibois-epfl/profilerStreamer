#include "ProfilerStreamer.hh"


int main()
{
    // From BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2/example/src/oxapiexamples.cpp
    // create an instance of a Ox object
    std::string TCPIPhost = "192.168.0.251";
    std::string OPCUAHost = "opc.tcp://192.168.0.64:4840";

    ProfilerStreaming::Communicate::TCPCommunicator tcpCommunicator(TCPIPhost, DeviceType::OX);
    ProfilerStreaming::Communicate::OPCUACommunicator opcuaCommunicator(OPCUAHost, DeviceType::IO_LINK, {6, 229916});
    tcpCommunicator.Connect();
    opcuaCommunicator.Connect();

    std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> profilesOverTime;
    std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> rangefinderDataOverTime;

    std::thread tcpThread([&tcpCommunicator, &profilesOverTime]() 
    {
        int counter = 0;
        while (counter < 5)
        {
            ProfilerStreaming::SpatialData::PointCloudWithTimestamp profileWithTimestamp 
                = tcpCommunicator.GetDataWithTimestamp();
            profilesOverTime.push_back(profileWithTimestamp);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            counter++;
        }
    });
    tcpThread.detach();

    std::thread opcuaThread([&opcuaCommunicator, &rangefinderDataOverTime]()
    {
        int counter = 0;
        while (counter < 5)
        {
            ProfilerStreaming::SpatialData::PointCloudWithTimestamp rangefinderDataWithTimestamp 
                = opcuaCommunicator.GetDataWithTimestamp();
            rangefinderDataOverTime.push_back(rangefinderDataWithTimestamp);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            counter++;
        }
    });
    opcuaThread.detach();

    std::cout << "Recieved " << profilesOverTime.size() << " profiles and " 
        << rangefinderDataOverTime.size() << " distance data points";
    return 0;
}