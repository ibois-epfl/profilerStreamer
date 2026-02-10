#include <iostream>
#include <stdio.h>
#include <open3d/Open3D.h>
#include <open3d/geometry/PointCloud.h>
#include <open3d/geometry/Geometry3D.h>
#include <boost/thread.hpp>
#include <OXApi/Ox.h>
#include <open62541/client.h>
#include <open62541/client_highlevel.h>
#include <direct.h> // for _getcwd on Windows
#include <chrono>
#include <thread>
#include <map>


int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::chrono::high_resolution_clock::time_point, uint16_t> caughtValues;
    // From BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2/example/src/oxapiexamples.cpp
    // create an instance of a Ox object
    std::string host = "192.168.0.251";
    auto ox = Baumer::OXApi::Ox::Create(host);
    std::string outputFile = "image.pgm";

    UA_Client *client = UA_Client_new();
    UA_StatusCode status = UA_Client_connect(client, "opc.tcp://192.168.0.64:4840");
    if(status != UA_STATUSCODE_GOOD) 
    {
        UA_Client_delete(client);
        return status;
    }

    uint16_t caughtValue;

    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);
    // UA_NodeId processValueNodeId = UA_NODEID_STRING(3, "Pin4ProcessData");
    UA_NodeId processValueNodeId = UA_NODEID_NUMERIC(6, 229916);
    status = UA_Client_readValueAttribute(client, processValueNodeId, &value);
    if(status == UA_STATUSCODE_GOOD) 
    {
        // Adjust the type check to match the expected data type (e.g., INT32, BYTE, etc.)
        if(UA_Variant_hasArrayType(&value, &UA_TYPES[UA_TYPES_BYTE])) 
        {
            UA_Byte* data = (UA_Byte*)value.data;
            size_t len = value.arrayLength;
            std::cout << "Port process value (byte array, length " << len << "): ";
            for(size_t i = 0; i < len; ++i) 
            {
                printf("%02X ", data[i]);
            }
            std::cout << std::endl;

            // Example: Convert first 2 bytes to a 16-bit integer (little-endian)
            if(len >= 2) 
            {
                uint16_t value_mm = (data[0] << 8) | data[1];
                std::cout << "Distance: " << value_mm << " mm" << std::endl;
                caughtValue = value_mm;
            }
        }
    } 
    else 
    {
        printf("Failed to read process value, status code: %08x\n", status);
    }

    std::shared_ptr<open3d::geometry::PointCloud> O3DPointCloud(new open3d::geometry::PointCloud());
    try
    {
        // opens the network connection
        ox->Connect();

        // get the latest sensor camera image
        auto profile = ox->GetProfile();
        std::vector<Eigen::Vector3d> points;
        for(u_int i = 0; i < profile.Length; i++)
        {
            if(profile.X.at(i) == 0 && profile.Z.at(i) == 0)
                continue; // skip invalid points
            else if (profile.X.at(i) && profile.Z.at(i))
            {
                double x = profile.X.at(i) / 1000.0;
                double y = i / 1000.0; // Add small y variation
                double z = profile.Z.at(i) / 1000.0;
                points.emplace_back(x, y, z);
                std::cout << "populated point: {" << x << ", " << y << ", " << z << "}" << std::endl;
            }
        }
        O3DPointCloud->points_ = points;
    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // closes the network connection
    ox->Disconnect();

    UA_Variant_clear(&value);
    UA_Client_delete(client);

    for (const auto& point : O3DPointCloud->points_)
    {
        std::cout << "Point: {" << point.x() << ", " << point.y() << ", " << point.z() << "}" << std::endl;
    }

    // Print typeid of first point
    if (!O3DPointCloud->IsEmpty()) {
        std::cout << "Type of first point: " << typeid(O3DPointCloud->points_[0]).name() << std::endl;
    }
    // Print current working directory
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("_getcwd() error");
    }

    bool success = open3d::io::WritePointCloudToPLY("test.ply", *O3DPointCloud, open3d::io::WritePointCloudOption());
    if (success) 
    {
        std::cout << "Point cloud saved successfully as PLY." << std::endl;
    } else 
    {
        std::cerr << "Failed to save point cloud as PLY." << std::endl;
    }

    return 0;
}