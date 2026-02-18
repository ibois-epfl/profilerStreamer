#pragma once

// STL headers
#include <vector>
#include <string>
#include <utility>
#include <chrono>

// Project headers
#include "../Device/Device.hh"
#include "../Data/Data.hh"

namespace ProfilerStreaming::PostProcess
{
    class DataSlicer
    {
        public:

            /*
            The constructor gets the "raw" data as parameter, that will be post-processed
            */
            DataSlicer(std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> measuredProfiles,
                    std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> measuredRangeFinderDistances);

            ~DataSlicer() = default;

            /*
            Slices the data in segments situated between the min and max that are returned by the function
            This is intended to detect when the profiler changed direction and thus when the b-axis rotated
            
            @param measurmentIntervalInMilliseconds: the time interval in milliseconds that is used to determine which data points belong to the same segment.

            @return std::pair<double minThreshold, double maxThreshold>
            */
            std::pair<double, double> Slice(int measurmentIntervalInMilliseconds);

            std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> GetProfilesSortedIntoSegments() 
                const { return this->profilesSortedIntoSegments; };
            std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> GetRangeFinderDistancesSortedIntoSegments() 
                const { return this->rangeFinderDistancesSortedIntoSegments; };
            /*
            This method computes regularized profiles by applying a correction to the profile data based on the rangefinder data, 
            to account for the movement of the profiler during scanning. 
            It assumes that the profiles and rangefinder data have already been sorted into segments using the Slice method.

            @return std::vector<Eigen::Vector3d> a vector of regularized profiles, 
                    where each profile has been corrected based on the corresponding rangefinder data.
            */
            std::vector<Eigen::Vector3d> ComputeRegularizedProfiles();

            // a few getters
            uint8_t GetNumberOfSegments() const { return this->numberOfSegments; }
            double GetMinThreshold() const { return this->minThreshold; }
            double GetMaxThreshold() const { return this->maxThreshold; }
        
        private:
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> unsortedProfiles;
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> unsortedRangeFinderDistances;

            std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> profilesSortedIntoSegments;
            std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> rangeFinderDistancesSortedIntoSegments;

            uint8_t numberOfSegments = 0;
            double minThreshold = 0;
            double maxThreshold = 0;
    };
}

