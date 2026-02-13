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

    std::thread tcpThread([&tcpCommunicator]() {
        while (true) {
            tcpCommunicator.GetDataWithTimestamp();
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust the sleep duration as needed
        }
    });
    int counter = 0;
    while(counter < 5) // Loop to get data multiple times for testing
    {
        std::chrono::milliseconds delay(1000); // 1 second delay to allow devices to initialize and start streaming data
        std::this_thread::sleep_for(delay);
        std::pair<std::vector<Eigen::Vector3d>, std::chrono::time_point<std::chrono::system_clock>> dataWithTimestamp 
            = tcpCommunicator.GetDataWithTimestamp();
        std::pair<std::vector<Eigen::Vector3d>, std::chrono::time_point<std::chrono::system_clock>> opcuaDataWithTimestamp 
            = opcuaCommunicator.GetDataWithTimestamp();
        std::cout << "Received " << dataWithTimestamp.first.size() << " points from TCPCommunicator at timestamp " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(dataWithTimestamp.second.time_since_epoch()).count() 
                << " ms since epoch." << std::endl;

        std::cout << "Received " << opcuaDataWithTimestamp.first.size() << " points from OPCUACommunicator at timestamp " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(opcuaDataWithTimestamp.second.time_since_epoch()).count() 
                << " ms since epoch." << std::endl;
        counter++;
    }

    return 0;
}