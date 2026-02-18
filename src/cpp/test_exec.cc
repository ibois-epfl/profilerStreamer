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
    bool hasStopped = false;
    std::thread tcpThread([&tcpCommunicator, &profilesOverTime, &hasStopped]() 
    {
        while (! hasStopped)
        {
            ProfilerStreaming::SpatialData::PointCloudWithTimestamp profileWithTimestamp 
                = tcpCommunicator.GetDataWithTimestamp();
            profilesOverTime.push_back(profileWithTimestamp);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });
    tcpThread.detach();

    std::thread opcuaThread([&opcuaCommunicator, &rangefinderDataOverTime, &hasStopped]()
    {
        while (! hasStopped)
        {
            ProfilerStreaming::SpatialData::PointCloudWithTimestamp rangefinderDataWithTimestamp 
                = opcuaCommunicator.GetDataWithTimestamp();
            rangefinderDataOverTime.push_back(rangefinderDataWithTimestamp);
        }
    });
    opcuaThread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(60)); 
    hasStopped = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 

    std::cout << "Recieved " << profilesOverTime.size() << " profiles and " 
        << rangefinderDataOverTime.size() << " distance data points" << std::endl;

    int nPointsInFirstProfile = profilesOverTime.size() > 0 ? profilesOverTime.at(0).GetNumPoints() : 0;
    int nPointsInLastProfile = profilesOverTime.size() > 0 ? profilesOverTime.at(profilesOverTime.size() - 1).GetNumPoints() : 0;
    
    ProfilerStreaming::PostProcess::DataSlicer dataSlicer(profilesOverTime, rangefinderDataOverTime);
    std::pair<double, double> thresholds = dataSlicer.Slice(1000);
    std::vector<Eigen::Vector3d> regularizedProfile = dataSlicer.ComputeRegularizedProfiles();
    open3d::geometry::PointCloud combinedPointCloud = ProfilerStreaming::SpatialData::PCWT2Open3DConverter::Convert(regularizedProfile);
    std::cout << "Combined point cloud has " << combinedPointCloud.points_.size() << " points." << std::endl;
    open3d::io::WritePointCloud("combined_point_cloud.ply", combinedPointCloud);
    return 0;
}