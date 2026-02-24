#include "ProfilerStreamer.hh"


int main(int argc, char* argv[])
{
    // From BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2/example/src/oxapiexamples.cpp
    // create an instance of a Ox object
    std::string TCPIPhost = "192.168.0.251";
    std::string OPCUAHost = "opc.tcp://192.168.0.64:4840";

    ProfilerStreaming::Communicate::TCPCommunicator tcpCommunicator(TCPIPhost, ProfilerStreaming::Device::DeviceType::OX);
    ProfilerStreaming::Communicate::OPCUACommunicator opcuaCommunicator(OPCUAHost, ProfilerStreaming::Device::DeviceType::IO_LINK, {6, 229916});
    tcpCommunicator.Connect();
    opcuaCommunicator.Connect();

    std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> profilesOverTime;
    std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> rangefinderDataOverTime;
    std::atomic<bool> record = true;
    ProfilerStreaming::Communicate::TCPRecorder* tcpRecorder = new ProfilerStreaming::Communicate::TCPRecorder(tcpCommunicator, 5);
    ProfilerStreaming::Communicate::OPCUARecorder* opcuaRecorder = new ProfilerStreaming::Communicate::OPCUARecorder(opcuaCommunicator, 5);
    tcpRecorder->StartRecording();
    opcuaRecorder->StartRecording();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    tcpRecorder->StopRecording();
    opcuaRecorder->StopRecording();
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    profilesOverTime = tcpRecorder->GetRecordedData();
    rangefinderDataOverTime = opcuaRecorder->GetRecordedData();

    std::cout << "Recieved " << profilesOverTime.size() << " profiles and " 
        << rangefinderDataOverTime.size() << " distance data points" << std::endl;

    int nPointsInFirstProfile = profilesOverTime.size() > 0 ? profilesOverTime.at(0).GetNumPoints() : 0;
    int nPointsInLastProfile = profilesOverTime.size() > 0 ? profilesOverTime.at(profilesOverTime.size() - 1).GetNumPoints() : 0;
    ProfilerStreaming::PostProcess::DataSlicer dataSlicer(profilesOverTime, rangefinderDataOverTime);
    std::pair<double, double> thresholds = dataSlicer.Slice(1000);
    Eigen::Matrix3d rotationXAxis = Eigen::AngleAxisd(1.5 * M_PI / 180, Eigen::Vector3d::UnitX()).toRotationMatrix();
    Eigen::Matrix4d firstTransformationMatrix = Eigen::Matrix4d::Identity();
    firstTransformationMatrix.block<3,3>(0,0) = rotationXAxis;
    Eigen::Matrix4d transformationMatrix = Eigen::Matrix4d::Identity();
    double y = -1758.07;
    double z = 555.30;
    transformationMatrix(1, 1) = -1;
    transformationMatrix(0, 0) = -1;
    transformationMatrix(0, 3) = 4834;
    transformationMatrix(1, 3) = y-339.63;
    transformationMatrix(2, 3) = z-362.92;
    std::vector<Eigen::Vector3d> regularizedProfile = dataSlicer.ComputeRegularizedProfiles();
    for (auto& point : regularizedProfile)
    {
        point = (transformationMatrix * firstTransformationMatrix * point.homogeneous()).head<3>();
    }
    open3d::geometry::PointCloud combinedPointCloud = ProfilerStreaming::SpatialData::PCWT2Open3DConverter::Convert(regularizedProfile);
    std::cout << "Combined point cloud has " << combinedPointCloud.points_.size() << " points." << std::endl;
    open3d::io::WritePointCloud("combined_point_cloud.ply", combinedPointCloud);
    return 0;
}