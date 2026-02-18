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
        
        bool switchFlag = false;

        // Sort the profiles into the detected segments
        for (int i = 0; i < this->unsortedRangeFinderDistances.size() - nIntervals; ++i)
        {
            double distanceDerivative = (this->unsortedRangeFinderDistances.at(i+nIntervals).GetPoints().at(0).x() 
                                                  - this->unsortedRangeFinderDistances.at(i).GetPoints().at(0).x()) 
                                        / std::chrono::duration_cast<std::chrono::milliseconds>(this->unsortedRangeFinderDistances.at(i+nIntervals).GetTimestamp() 
                                                                                              - this->unsortedRangeFinderDistances.at(i).GetTimestamp()).count();
            if (std::abs(distanceDerivative) < 0.005 && rangeFinderDistanceSegments.size() == 0) // IE if in measurmentIntervalInMilliseconds ms the speed was under 5mm/second, we assume no movement.
            {
                // in this case, we haven't started to actually scan.
                continue;
            }
            else
            {
                if (std::abs(distanceDerivative) < 0.005 && switchFlag == false)
                {
                    rangeFinderDistanceSegments.push_back({});
                    switchFlag = true;
                }
                else if (std::abs(distanceDerivative) < 0.005 && switchFlag == true)
                {
                    continue;
                }
                else if (std::abs(distanceDerivative) >= 0.005)
                {
                    if (rangeFinderDistanceSegments.size() == 0)
                    {
                        rangeFinderDistanceSegments.push_back({});
                    }
                    rangeFinderDistanceSegments.back().push_back(this->unsortedRangeFinderDistances.at(i));
                    switchFlag = false;
                }
            }
        }
        this->rangeFinderDistancesSortedIntoSegments = rangeFinderDistanceSegments;
        this->numberOfSegments = rangeFinderDistanceSegments.size();
        std::cout << "Sorted rangefinder data into " << numberOfSegments << " segments." << std::endl;
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

    std::vector<Eigen::Vector3d> DataSlicer::ComputeRegularizedProfiles()
    {
        std::vector<Eigen::Vector3d> regularizedProfiles;

        // TODO: Add rotation around axis for each slice.
        for (int i = 0; i < this->rangeFinderDistancesSortedIntoSegments.size(); ++i)
        {
            double speed = (this->rangeFinderDistancesSortedIntoSegments.at(i).back().GetPoints().at(0).x() 
                            - this->rangeFinderDistancesSortedIntoSegments.at(i).front().GetPoints().at(0).x()) 
                            / std::chrono::duration_cast<std::chrono::milliseconds>(this->rangeFinderDistancesSortedIntoSegments.at(i).back().GetTimestamp() 
                                                                                    - this->rangeFinderDistancesSortedIntoSegments.at(i).front().GetTimestamp()).count();
            std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp> profileVector = this->profilesSortedIntoSegments.at(i);
            for (const auto& profileWithTimestamp : profileVector)
            {
                for (const auto& rangeFinderData : this->rangeFinderDistancesSortedIntoSegments.at(i))
                {
                    if (rangeFinderData.GetTimestamp() < profileWithTimestamp.GetTimestamp()) // IE if the rangefinder data is from before the profile data, and we are actually moving, we assume the profile data is valid and should be added to the point cloud.
                    {
                        continue;
                    }
                    else
                    {
                        std::chrono::duration<double> timeDifference = profileWithTimestamp.GetTimestamp() - rangeFinderData.GetTimestamp();
                        std::chrono::milliseconds timeDifferenceInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeDifference);
                        double correctionDistance = speed * timeDifferenceInMilliseconds.count();
                        for (const auto& point : profileWithTimestamp.GetPoints())
                        {
                            Eigen::Vector3d correctedPoint;
                            correctedPoint.x() = rangeFinderData.GetPoints().at(0).x() + correctionDistance;
                            correctedPoint.y() = point.x(); // because the x axis for the profiler is the y axis in the CNC.
                            correctedPoint.z() = point.z();
                            regularizedProfiles.push_back(correctedPoint);
                        }
                        break;
                    }
                }
            }
        }
        return regularizedProfiles;
    }
}