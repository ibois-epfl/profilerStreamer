#pragma once

// STL headers
#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <array>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

            /*
            Getter for the profiles sorted into segments.
            @return A vector of vectors of PointCloudWithTimestamp objects representing the profiles sorted into segments.
            */
            std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> GetProfilesSortedIntoSegments() 
                const { return this->profilesSortedIntoSegments; };
            /*
            Getter for the rangefinder distances sorted into segments.
            @return A vector of vectors of PointCloudWithTimestamp objects representing the rangefinder distances sorted into segments.
            */
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

            /*
            A python binding friendly version of ComputeRegularizedProfiles, that returns the regularized profiles as a vector of arrays of doubles, instead of a vector of Eigen::Vector3d, to avoid issues with binding Eigen types to Python.

            @return std::vector<std::vector<double>> a vector of regularized profiles, 
                    where each profile has been corrected based on the corresponding rangefinder data, and is represented as an array of doubles with 3 elements (x, y, z).
            */
            std::vector<std::vector<double>> ComputeRegularizedProfilesAsArray();

            // a few getters
            uint8_t GetNumberOfSegments() const { return this->numberOfSegments; }
            double GetMinThreshold() const { return this->minThreshold; }
            double GetMaxThreshold() const { return this->maxThreshold; }
            std::vector<double> GetCorrectionDistances() const { return this->correctionDistances; }
            
        
        private:
            /*
            The unsorted profiles, as a private member.
            */
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> unsortedProfiles;

            /*
            The unsorted rangefinder distances, as a private member.
            */
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> unsortedRangeFinderDistances;

            /*
            The profiles sorted into segments, as a private member.
            */
            std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> profilesSortedIntoSegments;
            /*
            The rangefinder distances sorted into segments, as a private member.
            */
            std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> rangeFinderDistancesSortedIntoSegments;

            /*
            The number of segments, as a private member.
            */
            uint8_t numberOfSegments = 0;

            /*
            The minimum value in the rangefinder values, as a private member.
            */
            double minThreshold = 0;
            /*
            The maximum value in the rangefinder values, as a private member.
            */
            double maxThreshold = 0;

            std::vector<double> correctionDistances;
    };
}

