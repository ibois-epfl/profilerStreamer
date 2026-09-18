#ifndef MOCK_COMMUNICATORS_HH
#define MOCK_COMMUNICATORS_HH

#pragma once

// STL headers
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

// Eigen for Vector3d
#include <Eigen/Dense>

// Project headers
#include "../Device/Device.hh"
#include "../Data/Data.hh"
#include "Communicate.hh" // For Chronometer

namespace ProfilerStreaming::Communicate
{
    /**
     * Mock TCP communicator that generates dummy profile data without connecting to a real device.
     * Useful for testing the recording functionality without hardware.
     */
    class MockTCPCommunicator
    {
    public:
        MockTCPCommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType)
            : host(host), deviceType(deviceType), profileCounter(0)
        {
        }

        ~MockTCPCommunicator() = default;

        bool Connect() { return true; }
        bool Disconnect() { return true; }
        ProfilerStreaming::SpatialData::PointCloudWithTimestamp GetDataWithTimestamp()
        {
            // Generate a dummy profile with some points
            std::vector<Eigen::Vector3d> points;
            Chronometer& chronometer = Chronometer::GetInstance();
            auto timestamp = chronometer.GetCurrentTime();

            // Generate a simple sine wave pattern for testing
            const int numPoints = 100;
            const double amplitude = 50.0;
            const double frequency = 0.1;
            
            for (int i = 0; i < numPoints; ++i)
            {
                double x = static_cast<double>(i);
                double y = amplitude * std::sin(frequency * (profileCounter + i));
                double z = 0.0;
                points.emplace_back(x, y, z);
            }
            
            profileCounter++;
            return ProfilerStreaming::SpatialData::PointCloudWithTimestamp(points, timestamp);
        }

        ProfilerStreaming::Device::DeviceType GetDeviceType() const { return this->deviceType; }

    private:
        std::string host;
        ProfilerStreaming::Device::DeviceType deviceType;
        int profileCounter;
    };

    /**
     * Mock OPC-UA communicator that generates dummy distance data without connecting to a real server.
     * Useful for testing the recording functionality without hardware.
     */
    class MockOPCUACommunicator
    {
    public:
        MockOPCUACommunicator(const std::string& host, ProfilerStreaming::Device::DeviceType deviceType, std::pair<int, int> namespaceIndexAndIdentifier)
            : host(host), deviceType(deviceType), namespaceIndexAndIdentifier(namespaceIndexAndIdentifier), distanceValue(0)
        {
        }

        ~MockOPCUACommunicator() = default;

        bool Connect() { return true; }
        bool Disconnect() { return true; }
        ProfilerStreaming::SpatialData::PointCloudWithTimestamp GetDataWithTimestamp()
        {
            std::vector<Eigen::Vector3d> points;
            Chronometer& chronometer = Chronometer::GetInstance();
            auto timestamp = chronometer.GetCurrentTime();

            // Generate a simple oscillating distance value
            distanceValue = 1000 + static_cast<int>(500 * std::sin(0.1 * distanceValue));
            
            // Return as a single point (simulating the rangefinder)
            Eigen::Vector3d point(distanceValue, 0, 0);
            points.push_back(point);
            
            return ProfilerStreaming::SpatialData::PointCloudWithTimestamp(points, timestamp);
        }

    private:
        std::string host;
        ProfilerStreaming::Device::DeviceType deviceType;
        std::pair<int, int> namespaceIndexAndIdentifier;
        int distanceValue;
    };
}

#endif // MOCK_COMMUNICATORS_HH
