#include "Communicate/Communicate.hh"
#include "Communicate/MockOPCUAServer.hh"
#include "Data/Data.hh"
#include "Device/Device.hh"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace
{
    bool g_allChecksPassed = true;

    void Check(bool condition, const std::string& description)
    {
        std::cout << (condition ? "  [PASS] " : "  [FAIL] ") << description << std::endl;
        if (!condition)
        {
            g_allChecksPassed = false;
        }
    }

    int RunTest()
    {
        std::cout << "Starting OPC-UA server integration test..." << std::endl;

        const uint16_t port = 4842;
        const std::pair<int, int> namespaceIndexAndIdentifier(6, 229916);
        const std::string host = "opc.tcp://127.0.0.1:" + std::to_string(port);

        // Start a real open62541 server that streams an oscillating distance value,
        // just like a physical OPC-UA rangefinder would.
        ProfilerStreaming::Communicate::MockOPCUAServer server(port, namespaceIndexAndIdentifier);
        server.Start();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::cout << "\nTesting direct reads against the real OPCUACommunicator..." << std::endl;
        {
            ProfilerStreaming::Communicate::OPCUACommunicator opcuaCommunicator(
                host, ProfilerStreaming::Device::DeviceType::IO_LINK, namespaceIndexAndIdentifier);

            bool connected = false;
            try
            {
                connected = opcuaCommunicator.Connect();
            }
            catch (const std::exception& e)
            {
                std::cout << "  Connect() threw: " << e.what() << std::endl;
            }
            Check(connected, "OPCUACommunicator connected to the mock server");

            std::vector<double> readValues;
            const int numReads = 20;
            for (int i = 0; i < numReads; ++i)
            {
                auto data = opcuaCommunicator.GetDataWithTimestamp();
                auto points = data.GetPoints();
                Check(points.size() == 1, "read " + std::to_string(i) + " returned exactly one point");
                if (!points.empty())
                {
                    readValues.push_back(points[0].x());
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }

            bool allInRange = !readValues.empty();
            for (double v : readValues)
            {
                if (v < 500.0 || v > 1500.0)
                {
                    allInRange = false;
                    break;
                }
            }
            Check(allInRange, "all read values fall within the expected [500, 1500] range");

            bool valuesChanged = false;
            for (size_t i = 1; i < readValues.size(); ++i)
            {
                if (readValues[i] != readValues[0])
                {
                    valuesChanged = true;
                    break;
                }
            }
            Check(valuesChanged, "values change over time (proves data is actually streamed live, not cached)");

            opcuaCommunicator.Disconnect();
        }

        std::cout << "\nTesting OPCUARecorder against the real server..." << std::endl;
        {
            ProfilerStreaming::Communicate::OPCUACommunicator opcuaCommunicator(
                host, ProfilerStreaming::Device::DeviceType::IO_LINK, namespaceIndexAndIdentifier);
            opcuaCommunicator.Connect();

            ProfilerStreaming::Communicate::OPCUARecorder recorder(opcuaCommunicator, 15);
            recorder.StartRecording();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            recorder.StopRecording();

            auto recorded = recorder.GetRecordedData();
            std::cout << "  Recorded " << recorded.size() << " data points over 500ms" << std::endl;
            Check(recorded.size() >= 10, "recorder collected a reasonable number of samples");

            bool allNonEmpty = !recorded.empty();
            bool recordedValuesChanged = false;
            for (size_t i = 0; i < recorded.size(); ++i)
            {
                if (recorded[i].GetNumPoints() != 1)
                {
                    allNonEmpty = false;
                }
                if (i > 0 && !recorded[i].GetPoints().empty() && !recorded[0].GetPoints().empty() &&
                    recorded[i].GetPoints()[0].x() != recorded[0].GetPoints()[0].x())
                {
                    recordedValuesChanged = true;
                }
            }
            Check(allNonEmpty, "every recorded sample has exactly one point");
            Check(recordedValuesChanged, "recorded values change over time");

            opcuaCommunicator.Disconnect();
        }

        server.Stop();

        std::cout << "\n" << (g_allChecksPassed ? "OPC-UA server integration test PASSED" : "OPC-UA server integration test FAILED") << std::endl;
        return g_allChecksPassed ? 0 : 1;
    }
}

int main()
{
    // Run the whole test under a top-level catch: an uncaught exception here would
    // otherwise call std::terminate/abort(), which on Windows can swallow buffered
    // stdout entirely (no flush on abnormal termination) and show up in CI as a bare
    // "exit code 1" with no diagnostic at all.
    try
    {
        return RunTest();
    }
    catch (const std::exception& e)
    {
        std::cout << "\nOPC-UA server integration test FAILED with exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "\nOPC-UA server integration test FAILED with an unknown exception" << std::endl;
        return 1;
    }
}
