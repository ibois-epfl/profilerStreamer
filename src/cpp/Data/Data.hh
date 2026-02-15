#pragma once

#include <open3d/Open3D.h>
#include <Eigen/Dense>

namespace ProfilerStreaming::SpatialData
{
    class PointCloudWithTimestamp
    {
    public:
        PointCloudWithTimestamp() = default;
        PointCloudWithTimestamp(const std::vector<Eigen::Vector3d>& points, 
                                const std::chrono::time_point<std::chrono::system_clock>& timestamp)
            : points(points), timestamp(timestamp) {}

        const std::vector<Eigen::Vector3d>& GetPoints() const { return points; }
        const std::chrono::time_point<std::chrono::system_clock>& GetTimestamp() const { return timestamp; }
        int GetNumPoints() { return points.size(); }

    private:
        std::vector<Eigen::Vector3d> points;
        std::chrono::time_point<std::chrono::system_clock> timestamp;
    };
}