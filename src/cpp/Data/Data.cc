# include "Data.hh"

namespace ProfilerStreaming::SpatialData
{
    open3d::geometry::PointCloud PCWT2Open3DConverter::Convert(std::vector<std::vector<Eigen::Vector3d>>& slicedProfilerData)
    {
        open3d::geometry::PointCloud combinedPointCloud;
        for (const auto& profile : slicedProfilerData)
        {
            for (const auto& point : profile)
            {
                combinedPointCloud.points_.push_back(point);
            }
        }
        return combinedPointCloud;
    }
}
