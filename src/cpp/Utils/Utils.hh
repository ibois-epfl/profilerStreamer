#pragma once

#include <vector>
#include <chrono>
#include "../Data/Data.hh"

namespace ProfilerStreaming::Utils
{
    std::pair<double, double> linearRegressionSlope(const std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>& data, int referenceIndex);
}