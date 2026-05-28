#pragma once

#include <vector>
#include <chrono>
#include "../Data/Data.hh"

#include <Eigen/Dense>

namespace ProfilerStreaming::Utils
{
    std::pair<double, double> linearRegressionSlope(const std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>& data, int referenceIndex);

    Eigen::Matrix4d ComputeTransformationMatrix(const std::vector<Eigen::Vector3d>& sourcePoints, const std::vector<Eigen::Vector3d>& targetPoints);
}