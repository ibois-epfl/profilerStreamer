#include <iostream>
#include <stdio.h>
#include <boost/thread.hpp>
#include <OXApi/Ox.h>
#include <open62541/client.h>
#include <open62541/client_highlevel.h>

int main()
{
    // From BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2/example/src/oxapiexamples.cpp
    // create an instance of a Ox object
    std::string host = "192.168.0.251";
    auto ox = Baumer::OXApi::Ox::Create(host);
    std::string outputFile = "image.pgm";

    UA_Client *client = UA_Client_new();
    UA_StatusCode status = UA_Client_connect(client, "opc.tcp://192.168.0.64:4840");
    if(status != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return status;
    }

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
            }
        }
    } 
    else {
        printf("Failed to read process value, status code: %08x\n", status);
    }

    try
    {
        // opens the network connection
        ox->Connect();
        // login to get access to all configuration parameters
        ox->Login("admin", "");

        // get the latest sensor camera image
        auto image = ox->GetImage();

        // get additional information about the image
        auto imageInfo = ox->GetImageInfo();
        std::cout << "ImageInfo:" << std::endl;
        std::cout << "  SensorHeight[ " << imageInfo.SensorHeight << " ]" << std::endl;
        std::cout << "  SensorWidth[ " << imageInfo.SensorWidth << " ]" << std::endl;
        std::cout << "  MaxROIPixels[ " << imageInfo.MaxROIPixels << " ]" << std::endl;

        // save the image to the hard disk as PGM file.
        // use linux command to convert PGM file to PNG: "convert image.pgm image.png"
        ox->SaveImage( outputFile, image );

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // closes the network connection
    ox->Disconnect();

    UA_Variant_clear(&value);
    UA_Client_delete(client);
    return 0;
}