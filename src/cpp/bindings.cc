#include "nanobind/nanobind.h"
#include "nanobind/stl/string.h"
#include <nanobind/stl/pair.h>
#include <nanobind/stl/vector.h>
#include <nanobind/eigen/dense.h>

#include "ProfilerStreamer.hh"

NB_MODULE(profilerStreamerBindings, m)
{
    nanobind::enum_<ProfilerStreaming::Device::DeviceType>(m, "DeviceType")
        .value("OTHER", ProfilerStreaming::Device::DeviceType::OTHER, "Other device type, not specified")
        .value("OX", ProfilerStreaming::Device::DeviceType::OX, "Baumer OX device type")
        .value("IO_LINK", ProfilerStreaming::Device::DeviceType::IO_LINK, "IO-Link device type");

    nanobind::class_<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>(m, "PointCloudWithTimestamp")
        .def(nanobind::init<const std::vector<Eigen::Vector3d>&, const std::chrono::time_point<std::chrono::high_resolution_clock>&>())
        .def("GetPoints", &ProfilerStreaming::SpatialData::PointCloudWithTimestamp::GetPoints, "Returns the point cloud data as a vector of Eigen::Vector3d, where each Vector3d represents a point in 3D space with x, y, z coordinates.")
        .def("GetTimeStampAsInt", &ProfilerStreaming::SpatialData::PointCloudWithTimestamp::GetTimeStampAsInt, "Returns the timestamp as an integer representing the number of milliseconds since the epoch.")
        .def("GetNumPoints", &ProfilerStreaming::SpatialData::PointCloudWithTimestamp::GetNumPoints, "Returns the number of points in the point cloud.");

    nanobind::class_<ProfilerStreaming::Communicate::TCPCommunicator>(m, "TCPCommunicator")
        .def(nanobind::init<const std::string&, ProfilerStreaming::Device::DeviceType>())
        .def("Connect", &ProfilerStreaming::Communicate::TCPCommunicator::Connect, "Establishes a TCP connection to the device using the provided IP address and device type.")
        .def("Disconnect", &ProfilerStreaming::Communicate::TCPCommunicator::Disconnect, "Closes the TCP connection to the device.")
        .def("GetDataWithTimestamp", &ProfilerStreaming::Communicate::TCPCommunicator::GetDataWithTimestamp, "Retrieves the point cloud data along with its timestamp from the device.");

    nanobind::class_<ProfilerStreaming::Communicate::OPCUACommunicator>(m, "OPCUACommunicator")
        .def(nanobind::init<const std::string&, ProfilerStreaming::Device::DeviceType, std::pair<int, int>>())
        .def("Connect", &ProfilerStreaming::Communicate::OPCUACommunicator::Connect, "Establishes a connection to the OPC UA server using the provided host, device type, and node identification.")
        .def("Disconnect", &ProfilerStreaming::Communicate::OPCUACommunicator::Disconnect, "Closes the connection to the OPC UA server.")
        .def("GetDataWithTimestamp", &ProfilerStreaming::Communicate::OPCUACommunicator::GetDataWithTimestamp, "Retrieves the point cloud data along with its timestamp from the OPC UA server.");

    nanobind::class_<ProfilerStreaming::Communicate::TCPRecorder>(m, "TCPRecorder")
        .def(nanobind::init<ProfilerStreaming::Communicate::TCPCommunicator&, int>(), nanobind::arg("tcpCommunicator"), nanobind::arg("recordingIntervalInMilliseconds"), "Initializes the TCPRecorder with a reference to a TCPCommunicator and a recording interval in milliseconds.")
        .def("StartRecording", &ProfilerStreaming::Communicate::TCPRecorder::StartRecording, "Starts recording data from the TCP communicator.")
        .def("StopRecording", &ProfilerStreaming::Communicate::TCPRecorder::StopRecording, "Stops recording data from the TCP communicator.")
        .def("GetRecordedData", &ProfilerStreaming::Communicate::TCPRecorder::GetRecordedData, "Returns the recorded point cloud data along with their timestamps as a vector of PointCloudWithTimestamp objects.");

    nanobind::class_<ProfilerStreaming::Communicate::OPCUARecorder>(m, "OPCUARecorder")
        .def(nanobind::init<ProfilerStreaming::Communicate::OPCUACommunicator&, int>(), nanobind::arg("opcuaCommunicator"), nanobind::arg("recordingIntervalInMilliseconds"), "Initializes the OPCUARecorder with a reference to an OPCUACommunicator and a recording interval in milliseconds.")
        .def("StartRecording", &ProfilerStreaming::Communicate::OPCUARecorder::StartRecording, "Starts recording data from the OPC UA communicator.")
        .def("StopRecording", &ProfilerStreaming::Communicate::OPCUARecorder::StopRecording, "Stops recording data from the OPC UA communicator.")
        .def("GetRecordedData", &ProfilerStreaming::Communicate::OPCUARecorder::GetRecordedData, "Returns the recorded point cloud data along with their timestamps as a vector of PointCloudWithTimestamp objects.");

    nanobind::class_<ProfilerStreaming::PostProcess::DataSlicer>(m, "DataSlicer")
        .def(nanobind::init<const std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>&, const std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>&>())
        .def("Slice", &ProfilerStreaming::PostProcess::DataSlicer::Slice, nanobind::arg("measurmentIntervalInMilliseconds"), "Slices the data into segments based on the specified measurement interval in milliseconds, which is used to determine which data points belong to the same segment.")
        .def("GetProfilesSortedIntoSegments", &ProfilerStreaming::PostProcess::DataSlicer::GetProfilesSortedIntoSegments)
        .def("GetRangeFinderDistancesSortedIntoSegments", &ProfilerStreaming::PostProcess::DataSlicer::GetRangeFinderDistancesSortedIntoSegments)
        .def("ComputeRegularizedProfilesAsArray", &ProfilerStreaming::PostProcess::DataSlicer::ComputeRegularizedProfilesAsArray)
        .def("GetNumberOfSegments", &ProfilerStreaming::PostProcess::DataSlicer::GetNumberOfSegments)
        .def("GetMinThreshold", &ProfilerStreaming::PostProcess::DataSlicer::GetMinThreshold)
        .def("GetMaxThreshold", &ProfilerStreaming::PostProcess::DataSlicer::GetMaxThreshold)
        .def("GetCorrectionDistances", &ProfilerStreaming::PostProcess::DataSlicer::GetCorrectionDistances);

    nanobind::module_ utils_submodule = m.def_submodule("Utils", "A submodule of 'ProfilerStreaming' for utility functions");
        utils_submodule.def("ComputeTransformationMatrix", &ProfilerStreaming::Utils::ComputeTransformationMatrix, nanobind::arg("sourcePoints"), nanobind::arg("targetPoints"), "Computes a transformation matrix that maps the source points to the target points using the Umeyama algorithm.");
}