#include "Communicate/MockCommunicators.hh"
#include "Data/Data.hh"

#include <iostream>
#include <vector>

int main()
{
    std::cout << "Starting mock stream test..." << std::endl;

    // Create mock communicators
    ProfilerStreaming::Communicate::MockTCPCommunicator mockTcpComm("127.0.0.1", ProfilerStreaming::Device::DeviceType::OX);
    ProfilerStreaming::Communicate::MockOPCUACommunicator mockOpuaComm("opc.tcp://127.0.0.1:4840", 
                                                               ProfilerStreaming::Device::DeviceType::IO_LINK, 
                                                               std::make_pair(6, 229916));

    // Connect (always succeeds for mocks)
    bool tcpConnected = mockTcpComm.Connect();
    bool opuaConnected = mockOpuaComm.Connect();
    
    std::cout << "Mock TCP connected: " << tcpConnected << std::endl;
    std::cout << "Mock OPC-UA connected: " << opuaConnected << std::endl;

    // Test direct data generation
    std::cout << "\nTesting direct data generation..." << std::endl;
    
    // Test TCP communicator - should generate 100 points in a sine wave
    auto tcpData1 = mockTcpComm.GetDataWithTimestamp();
    auto tcpData2 = mockTcpComm.GetDataWithTimestamp();
    
    std::cout << "TCP Profile 1: " << tcpData1.GetNumPoints() << " points" << std::endl;
    std::cout << "TCP Profile 2: " << tcpData2.GetNumPoints() << " points" << std::endl;
    
    // Verify TCP data has expected sine wave pattern
    auto tcpPoints1 = tcpData1.GetPoints();
    if (!tcpPoints1.empty()) {
        std::cout << "  First point: (" << tcpPoints1[0].x() << ", " << tcpPoints1[0].y() << ", " << tcpPoints1[0].z() << ")" << std::endl;
        std::cout << "  Last point: (" << tcpPoints1.back().x() << ", " << tcpPoints1.back().y() << ", " << tcpPoints1.back().z() << ")" << std::endl;
    }
    
    // Test OPC-UA communicator - should generate 1 point with oscillating distance
    auto opuaData1 = mockOpuaComm.GetDataWithTimestamp();
    auto opuaData2 = mockOpuaComm.GetDataWithTimestamp();
    
    std::cout << "OPC-UA Data 1: " << opuaData1.GetNumPoints() << " points" << std::endl;
    std::cout << "OPC-UA Data 2: " << opuaData2.GetNumPoints() << " points" << std::endl;
    
    // Verify OPC-UA data
    auto opuaPoints1 = opuaData1.GetPoints();
    if (!opuaPoints1.empty()) {
        std::cout << "  Distance: " << opuaPoints1[0].x() << std::endl;
    }

    // Simulate recording for a few iterations
    std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> tcpProfiles;
    std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> opuaData;
    
    const int numIterations = 10;
    for (int i = 0; i < numIterations; ++i)
    {
        tcpProfiles.push_back(mockTcpComm.GetDataWithTimestamp());
        opuaData.push_back(mockOpuaComm.GetDataWithTimestamp());
    }
    
    std::cout << "\nRecorded " << tcpProfiles.size() << " TCP profiles" << std::endl;
    std::cout << "Recorded " << opuaData.size() << " OPC-UA data points" << std::endl;
    
    // Verify timestamps never move backwards.
    bool timestampsNondecreasing = true;
    for (size_t i = 1; i < tcpProfiles.size(); ++i) {
        if (tcpProfiles[i].GetTimestamp() < tcpProfiles[i - 1].GetTimestamp()) {
            timestampsNondecreasing = false;
            break;
        }
    }
    std::cout << "TCP timestamps are nondecreasing: "
              << (timestampsNondecreasing ? "YES" : "NO") << std::endl;

    mockTcpComm.Disconnect();
    mockOpuaComm.Disconnect();

    const bool checksPassed = tcpConnected && opuaConnected &&
        tcpData1.GetNumPoints() == 100 && tcpData2.GetNumPoints() == 100 &&
        opuaData1.GetNumPoints() == 1 && opuaData2.GetNumPoints() == 1 &&
        tcpProfiles.size() == numIterations && opuaData.size() == numIterations &&
        timestampsNondecreasing;
    std::cout << "\nMock stream test "
              << (checksPassed ? "completed successfully!" : "FAILED!") << std::endl;
    return checksPassed ? 0 : 1;
}
