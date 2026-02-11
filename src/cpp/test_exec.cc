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
    int xOffset = 5395;
    uint16_t timeOutSeconds = 20;
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

    uint16_t caughtXValue;
    std::vector<Eigen::Vector3d> points;

    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);
    // UA_NodeId processValueNodeId = UA_NODEID_STRING(3, "Pin4ProcessData");
    UA_NodeId processValueNodeId = UA_NODEID_NUMERIC(6, 229916);
    std::shared_ptr<open3d::geometry::PointCloud> O3DPointCloud(new open3d::geometry::PointCloud());
    ox->Connect();

    while(std::chrono::high_resolution_clock::now() - start < std::chrono::seconds(timeOutSeconds)) 
    {
        status = UA_Client_readValueAttribute(client, processValueNodeId, &value);
        if(status == UA_STATUSCODE_GOOD) 
        {
            // Adjust the type check to match the expected data type (e.g., INT32, BYTE, etc.)
            if(UA_Variant_hasArrayType(&value, &UA_TYPES[UA_TYPES_BYTE])) 
            {
                UA_Byte* data = (UA_Byte*)value.data;
                size_t len = value.arrayLength;
                if(len >= 2) 
                {
                    uint16_t value_mm = (data[0] << 8) | data[1];
                    value_mm = -value_mm + xOffset;
                    std::cout << "Distance: " << value_mm << " mm" << std::endl;
                    caughtXValue = value_mm;
                }
            }
        } 
        else 
        {
            printf("Failed to read process value, status code: %08x\n", status);
        }
    
        try
        {
            // get the latest sensor camera image
            auto profile = ox->GetProfile();
            auto info = ox->GetProfileInfo();
            int test = profile.XStart;
            std::cout << test << std::endl;
            
            for(u_int i = 0; i < profile.Length; i++)
            {
                if(profile.X.at(i) == 0 && profile.Z.at(i) == 0)
                    continue; // skip invalid points
                else if (profile.X.at(i) && profile.Z.at(i))
                {
                    double y = (profile.X.at(i) + profile.XStart) / (double)profile.Precision;
                    double x = caughtXValue;
                    double z = profile.Z.at(i) / (double)profile.Precision;
                    points.emplace_back(x, y, z);
                }
            }
            
        } catch (std::exception const &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    // closes the network connection
    ox->Disconnect();

    UA_Variant_clear(&value);
    UA_Client_delete(client);
    O3DPointCloud->points_ = points;
    open3d::visualization::DrawGeometries({O3DPointCloud}, "Point Cloud from Baumer Sensor", 800, 600);
    open3d::io::WritePointCloud("pointcloud.ply", *O3DPointCloud);

    return 0;
}