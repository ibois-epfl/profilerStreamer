#pragma once

#include <open3d/Open3D.h>
#include <Eigen/Dense>

namespace ProfilerStreaming::SpatialData
{
    /*
    A class representing a point cloud as a vector of Eigen::Vector3d, with an associated timestamp.
    */
    class PointCloudWithTimestamp
    {
    public:
        PointCloudWithTimestamp() = default;
        /*
        Constructor for the PointCloudWithTimestamp class.
        @param points A vector of Eigen::Vector3d representing the points in the point cloud.
        @param timestamp A time point representing the timestamp associated with the point cloud.
        */
        PointCloudWithTimestamp(const std::vector<Eigen::Vector3d>& points, 
                                const std::chrono::time_point<std::chrono::high_resolution_clock>& timestamp)
            : points(points), timestamp(timestamp) {}
        
        /*
        Getter for the points in the point cloud.
        @return A vector of Eigen::Vector3d representing the points in the point cloud.
        */
        const std::vector<Eigen::Vector3d>& GetPoints() const { return points; }

        /*
        Getter for the timestamp associated with the point cloud.
        @return A time point representing the timestamp.
        */
        const std::chrono::time_point<std::chrono::high_resolution_clock>& GetTimestamp() const { return timestamp; }

        /*
        Getter for the timestamp as an integer representing the number of milliseconds since the epoch. It is friendlier for python bindings.
        @return A long long integer representing the timestamp in milliseconds since the epoch.
        */
        long long GetTimeStampAsInt() const {return std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count(); }

        /*
        Getter for the number of points in the point cloud.
        @return An integer representing the number of points in the point cloud.
        */
        int GetNumPoints() { return points.size(); }

    private:
        /*
        The points in the point cloud, as a private member.
        */
        std::vector<Eigen::Vector3d> points;
        /*
        The timestamp associated with the point cloud, as a private member.
        */
        std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
    };


    class PCWT2Open3DConverter
    {
        public:
            static open3d::geometry::PointCloud Convert(std::vector<std::vector<Eigen::Vector3d>>& regularizedProfiles);
    };
}