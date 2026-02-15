# include "Data.hh"

namespace ProfilerStreaming::SpatialData
{
    open3d::geometry::PointCloud PCWT2Open3DConverter::Convert(std::vector<Eigen::Vector3d>& slicedProfilerData)
    {
        open3d::geometry::PointCloud combinedPointCloud;
        for (const auto& profile : slicedProfilerData)
        {
            combinedPointCloud.points_.push_back(profile);
        }
        return combinedPointCloud;
    }
}
