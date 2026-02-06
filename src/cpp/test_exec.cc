#include <iostream>
#include <boost/thread.hpp>
#include <OXApi/Ox.h>

int main()
{
    // From BaumerSDK/Cpp/LibOxApi_V2_0_2/LibOxApi_V2_0_2/example/src/oxapiexamples.cpp
    // create an instance of a Ox object
    std::string host = "192.168.0.251";
    auto ox = Baumer::OXApi::Ox::Create(host);
    std::string outputFile = "image.pgm";

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
    return 0;
}