# include "PostProcess.hh"

namespace ProfilerStreaming::PostProcess
{
    DataSlicer::DataSlicer(std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> measuredProfiles,
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> measuredRangeFinderDistances) : 
                unsortedProfiles(measuredProfiles), unsortedRangeFinderDistances(measuredRangeFinderDistances)
    {
        this->unsortedProfiles = measuredProfiles;
        this->unsortedRangeFinderDistances = measuredRangeFinderDistances;
        this->profilesSortedIntoSegments = {};
        this->rangeFinderDistancesSortedIntoSegments = {};
    }

    std::pair<double, double> DataSlicer::Slice(int measurmentIntervalInMilliseconds)
    {
        std::chrono::milliseconds measurmentInterval(measurmentIntervalInMilliseconds);
        int nIntervals = 0;
        std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> profileSegments = {};
        std::vector<std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>> rangeFinderDistanceSegments = {};

        // Detect the n measurments made in measurmentIntervalInMilliseconds miliseconds.
        for (size_t i = 1; i < this->unsortedRangeFinderDistances.size(); ++i)
        {
            auto timeDifference = this->unsortedRangeFinderDistances.at(i).GetTimestamp() - this->unsortedRangeFinderDistances.front().GetTimestamp();
            auto timeDifferenceInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeDifference).count();
            if (timeDifferenceInMilliseconds > measurmentIntervalInMilliseconds)
            {
                nIntervals = i;
                break;
            }
        }
        
        bool isIdle = false;

        // Sort the profiles into the detected segments
        for (int i = 0; i < this->unsortedRangeFinderDistances.size() - nIntervals; ++i)
        {
            double speed = (this->unsortedRangeFinderDistances.at(i+nIntervals).GetPoints().at(0).x() 
                                                  - this->unsortedRangeFinderDistances.at(i).GetPoints().at(0).x()) 
                                        / std::chrono::duration_cast<std::chrono::milliseconds>(this->unsortedRangeFinderDistances.at(i+nIntervals).GetTimestamp() 
                                                                                              - this->unsortedRangeFinderDistances.at(i).GetTimestamp()).count();
            if (std::abs(speed) < 0.005 && rangeFinderDistanceSegments.size() == 0) // IE if in measurmentIntervalInMilliseconds ms the speed was under 5mm/second, we assume no movement.
            {
                // in this case, we haven't started to actually scan.
                continue;
            }
            else
            {
                if (std::abs(speed) < 0.005 && isIdle == false)
                {
                    rangeFinderDistanceSegments.push_back({});
                    isIdle = true;
                }
                else if (std::abs(speed) < 0.005 && isIdle == true)
                {
                    continue;
                }
                else if (std::abs(speed) >= 0.005)
                {
                    if (rangeFinderDistanceSegments.size() == 0)
                    {
                        rangeFinderDistanceSegments.push_back({});
                    }
                    rangeFinderDistanceSegments.back().push_back(this->unsortedRangeFinderDistances.at(i));
                    isIdle = false;
                }
            }
        }
        
        // Just removing unvalid segments
        for (int i = 0; i < rangeFinderDistanceSegments.size(); ++i)
        {
            if (rangeFinderDistanceSegments.at(i).size() < 10) // if we have less than 10 rangefinder data points in a segment, we assume this is not a valid segment, but just some noise in the data, and we ignore it.
            {
                rangeFinderDistanceSegments.erase(rangeFinderDistanceSegments.begin() + i);
                --i;
            }
        }
        
        this->rangeFinderDistancesSortedIntoSegments = rangeFinderDistanceSegments;
        this->numberOfSegments = rangeFinderDistanceSegments.size();
        for (int i = 0; i < rangeFinderDistanceSegments.size(); ++i)
        {
            const std::chrono::time_point startTime = rangeFinderDistanceSegments.at(i).front().GetTimestamp();
            const std::chrono::time_point endTime = rangeFinderDistanceSegments.at(i).back().GetTimestamp();
            this->profilesSortedIntoSegments.resize(rangeFinderDistanceSegments.size());

            for (const auto& profile: this->unsortedProfiles)
            {
                if (profile.GetTimestamp() >= startTime && profile.GetTimestamp() <= endTime)
                {
                    this->profilesSortedIntoSegments.at(i).push_back(profile);
                }
            }
        }
        // TODO: compute min and max thresholds based on the rangefinder data, to detect when the profiler changed direction and thus when the b-axis rotated.
        return std::make_pair(0,0);
    }

    std::vector<std::vector<Eigen::Vector3d>> DataSlicer::ComputeRegularizedProfiles()
    {
        std::vector<std::vector<Eigen::Vector3d>> allRegularizedProfiles = {};

        for (int i = 0; i < this->rangeFinderDistancesSortedIntoSegments.size(); ++i)
        {
            std::vector<Eigen::Vector3d> regularizedProfiles;
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> profileVector = this->profilesSortedIntoSegments.at(i);
            int windowingSize = 3;
            for (int j = 0; j < profileVector.size() - 1; ++j)
            {
                auto tProfile = profileVector.at(j).GetTimestamp();
                for (int k = windowingSize; k < rangeFinderDistancesSortedIntoSegments.at(i).size() - windowingSize; ++k)
                {
                    if (rangeFinderDistancesSortedIntoSegments.at(i).at(k).GetTimestamp() < tProfile) // IE if the rangefinder data is from before the profile data, and we are actually moving, we assume the profile data is valid and should be added to the point cloud.
                    {
                        continue;
                    }
                    else
                    {
                        auto t_rangefinder = rangeFinderDistancesSortedIntoSegments.at(i).at(k).GetTimestamp();
                        std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> windowedRangeFinderData = {};
                        for (int l = k - windowingSize; l < k + windowingSize; ++l)
                        {
                            windowedRangeFinderData.push_back(rangeFinderDistancesSortedIntoSegments.at(i).at(l));
                        }
                        auto [slope, intercept] = ProfilerStreaming::Utils::linearRegressionSlope(windowedRangeFinderData, windowingSize);
                        double x_from_linear_regression = slope * std::chrono::duration_cast<std::chrono::microseconds>(t_rangefinder - tProfile).count() + intercept;
                        for (const auto& point : profileVector.at(j).GetPoints())
                        {
                            Eigen::Vector3d correctedPoint;
                            correctedPoint.x() = x_from_linear_regression / 10.0;
                            correctedPoint.y() = point.y();
                            correctedPoint.z() = point.z();
                            regularizedProfiles.push_back(correctedPoint);
                        }
                        this->correctionDistances.push_back(x_from_linear_regression / 10.0);
                        break;
                    }
                }
            }
            allRegularizedProfiles.push_back(regularizedProfiles);
        }
        return allRegularizedProfiles;
    }

    std::vector<std::vector<std::vector<double>>> DataSlicer::ComputeRegularizedProfilesAsArray()
    {
        std::vector<std::vector<std::vector<double>>> regularizedProfilesAsArray;
        std::vector<std::vector<Eigen::Vector3d>> regularizedProfilesIntoSegments = this->ComputeRegularizedProfiles();
        for (const std::vector<Eigen::Vector3d>& segment : regularizedProfilesIntoSegments)
        {
            std::vector<std::vector<double>> segmentAsArray;
            for (const auto& point : segment)
            {
                std::vector<double> pointAsArray = {point.x(), point.y(), point.z()};
                segmentAsArray.push_back(pointAsArray);
            }
            regularizedProfilesAsArray.push_back(segmentAsArray);
        }
        return regularizedProfilesAsArray;
    }
}