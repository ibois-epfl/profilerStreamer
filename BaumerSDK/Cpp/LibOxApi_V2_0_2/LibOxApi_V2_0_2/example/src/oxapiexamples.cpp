/*
 * oxapiexamples.cpp
 * Copyright: Baumer Group
 * Description : Example usage OxApi C++ SDK
 */

#include <OXApi/Ox.h>

#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm> // std::find_if
#include <thread>    // std::this_thread::sleep_for
#include <chrono>    // std::chrono::milliseconds
#include <stdio.h>
#include <fstream>
#include <csignal>
#include <sstream>

namespace
{
    volatile std::sig_atomic_t gSignalStatus;
}

void ExampleReadConfiguration(const std::string& host)
{
    // set output format
    std::cout.precision(2);
    std::cout << std::fixed;

    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host );

    try
    {
        // opens the network connection
        ox->Connect( );

        ox->Login( "admin" );

        /* Sensor information */
        auto sensorInfo = ox->GetSensorInfo( );
        std::cout << "SensorInfo: "
                    << " Type[" << sensorInfo.Type << "]"
                    << " SerialNumber[" << sensorInfo.SerialNumber << "]"
                    << " VendorName[" << sensorInfo.VendorName << "]"
                    << " AggregateVersion[" << sensorInfo.AggregateVersion << "]"
                    << " SoftwareVersion[" << sensorInfo.SoftwareVersion << "]"
                    << std::endl;

        /* Communication settings: Network Configuration */
        auto netConfig = ox->GetNetworkConfiguration( );
        std::cout << "NetworkConfiguration: "
                    << " DhcpActive[" << netConfig.DhcpActive << "]"
                    << " IpAddress[" << netConfig.IpAddress << "]"
                    << " SubnetMask[" << netConfig.SubnetMask << "]"
                    << " Gateway[" << netConfig.Gateway << "]"
                    << " MacAddress[" << netConfig.MacAddress << "]"
                    << std::endl;

        /* Communication settings: Time Servers */
        auto numTimeServer = ox->GetNumberOfTimeServers( );
        std::cout << "NumberOfTimeServers: " << numTimeServer << std::endl;

        auto timeServerConfiguration = ox->GetTimeServerConfiguration( );
        std::cout << "TimeServerConfiguration: [ ";
        for(auto timeserver : timeServerConfiguration.TimeServers) {
            std::cout << timeserver << " ";
        }
        std::cout << "]" << std::endl;

        /* Communication settings: Process Interfaces */
        auto processInterfacesInfo = ox->GetProcessInterfacesInfo( );
        std::cout << "Available RTEth Protocols: [";
        for(auto elem : processInterfacesInfo.RealtimeProtocols) {
            std::cout << " { id: " << elem.Id << ", name: " << elem.Name << " }";
        }
        std::cout << std::endl;

        auto processInterfaces = ox->GetProcessInterfaces( );
        std::cout << "ProcessInterfaces: "
                    << " ModbusEnabled[" << processInterfaces.ModbusEnabled << "]"
                    << " OPCUAEnabled[" << processInterfaces.OPCUAEnabled << "]"
                    << " RealtimeProtocol[" << processInterfaces.RealtimeProtocol << "]"
                    << " UdpStreamingEnabled[" << processInterfaces.UdpStreamingEnabled << "]"
                    << " UdpStreamingIp[" << processInterfaces.UdpStreamingIp << "]"
                    << " UdpStreamingPort[" << processInterfaces.UdpStreamingPort << "]"
                    << std::endl;

        auto udpInfo = ox->GetUdpStreamingInfo( );
        std::cout << "UdpStreamingInfo: [";
        for(auto elem : udpInfo.UdpStreams) {
            std::cout << " { id: " << elem.Id << ", name: " << elem.Name << " }";
        }
        std::cout << std::endl;

        auto activeStreams = ox->GetActiveUdpStreams( );
        std::cout << "ActiveUdpStreams: [";
        for(auto activeStream : activeStreams) {
            std::cout << " " << activeStream;
        }
        std::cout << " ]" << std::endl;

        /* Data acquisition: Laser Power */
        auto laserPowerLimits = ox->GetLaserPowerLimits( );
        std::cout << "LaserPowerLimits: "
                    << " MinFactor[" << laserPowerLimits.MinFactor << "]"
                    << " MaxFactor[" << laserPowerLimits.MaxFactor << "]";
        std::cout << " PredefinedFactors: [";
        for(auto factors : laserPowerLimits.PredefinedFactors) {
            std::cout << " " << factors;
        }
        std::cout << " ]" << std::dec << std::endl;

        auto laserPowerInfo = ox->GetLaserPowerInfo( );
        std::cout << "LaserPowerInfo: "
                    << " FactorUnit[" << laserPowerInfo.FactorUnit << "]"
                    << " FactorPrecision[" << laserPowerInfo.FactorPrecision << "]"
                    << std::endl;

        auto laserPower = ox->GetLaserPower( );
        std::cout << "LaserPower: " << laserPower << std::endl;

        /* Data acquisition: Exposure Time */
        auto exposureTimeLimits = ox->GetExposureTimeLimits( );
        std::cout << "ExposureTimeLimits: "
                    << " Minimum[" << exposureTimeLimits.Minimum << "]"
                    << " Maximum[" << exposureTimeLimits.Maximum << "]"
                    << std::endl;

        auto exposureTimeInfo = ox->GetExposureTimeInfo( );
        std::cout << "ExposureTimeInfo: "
                    << " Unit[" << exposureTimeInfo.Unit << "]"
                    << std::endl;

        auto exposureTime = ox->GetExposureTime( );
        std::cout << "ExposureTime: " << exposureTime << std::endl;

        /* Data acquisition: Trigger */
        auto triggerLimits = ox->GetTriggerLimits( );
        std::cout << "TriggerLimits: "
                    << " MinTime[" << triggerLimits.MinTime << "]"
                    << " MaxTime[" << triggerLimits.MaxTime << "]"
                    << " MinSteps[" << triggerLimits.MinSteps << "]"
                    << " MaxSteps[" << triggerLimits.MaxSteps << "]"
                    << std::endl;

        auto triggerInfo = ox->GetTriggerInfo( );
        std::cout << "TriggerInfo:" << std::endl;
        std::cout << "    TimeUnit: [" << triggerInfo.TimeUnit << "]" << std::endl;
        std::cout << "    TriggerModes: [" << std::endl;
        for(auto elem : triggerInfo.TriggerModes) {
            std::cout << "        { id: " << elem.Id << ", name: " << elem.Name << " }" << std::endl;
        }
        std::cout << "    ]" << std::endl;
        std::cout << "    TriggerOptions: [" << std::endl;
        for(auto elem : triggerInfo.TriggerOptions) {
            std::cout << "        { id: " << elem.Id << ", name: " << elem.Name << " }" << std::endl;
        }
        std::cout << "    ]" << std::endl;

        auto trigger = ox->GetTrigger( );
        std::cout << "Trigger: "
                    << " Mode[" << trigger.Mode << "]"
                    << " Option[" << trigger.Option << "]"
                    << " Time[" << trigger.Time << "]"
                    << " EncoderSteps[" << trigger.EncoderSteps << "]"
                    << std::endl;

        /* Data acquisition: Resolution */
        auto resolution = ox->GetResolution( );
        std::cout << "Resolution: "
                    << " XResolution[" << resolution.XResolution << "]"
                    << " ZResolution[" << resolution.ZResolution << "]"
                    << std::endl;

        auto resolutionInfo = ox->GetResolutionInfo( );
        std::cout << "ResolutionInfo:"  << std::endl;
        std::cout << "  XResolutions: [";
        for(auto value : resolutionInfo.XResolutions) {
            std::cout << " " << value;
        }
        std::cout << " ]" << std::endl;
        std::cout << "  ZResolutions: [";
        for(auto value : resolutionInfo.ZResolutions) {
            std::cout << " " << value;
        }
        std::cout << " ]" << std::endl;

        /* Profile configuration */
        auto resamplingEnabled = ox->IsResamplingEnabled();
        std::cout << "ResamplingEnabled: " << resamplingEnabled << std::endl;

        auto resamplingGrid = ox->GetResamplingGridValue( );
        std::cout << "ResamplingGrid: "
                    << " Enabled[" << resamplingGrid.Enabled << "]"
                    << " GridValue[" << resamplingGrid.GridValue << "]"
                    << std::endl;

        auto resamplingInfo = ox->GetResamplingInfo( );
        std::cout << "ResamplingInfo: "
                    << " GridUnit[" << resamplingInfo.GridUnit << "]"
                    << " GridPrecision[" << resamplingInfo.GridPrecision << "]"
                    << " MinimumGridValue[" << resamplingInfo.MinimumGridValue << "]"
                    << " MaximumGridValue[" << resamplingInfo.MaximumGridValue << "]"
                    << std::endl;

        auto profileFilter = ox->GetProfileFilter( );
        std::cout << "ProfileFilter: "
                    << " Enabled[" << profileFilter.Enabled << "]"
                    << " MovingAverageLength[" << profileFilter.MovingAverageLength << "]"
                    << std::endl;

        auto filterEnabled = ox->IsProfileFilterEnabled();
        std::cout << "ProfileFilterEnabled: " << filterEnabled << std::endl;

        auto profileFilterLimits = ox->GetProfileFilterLimits( );
        std::cout << "ProfileFilterLimits: "
                    << " MinimumLength[" << profileFilterLimits.MinimumLength << "]"
                    << " MaximumLength[" << profileFilterLimits.MaximumLength << "]"
                    << std::endl;

        auto profileAlgorithm = ox->GetProfileAlgorithm();
        std::cout << "ProfileAlgorithm: " << profileAlgorithm << std::endl;

        auto profileAlgorithms = ox->GetProfileAlgorithms( );
        std::cout << "ProfileAlgorithmInfo: [";
        for(auto algo : profileAlgorithms.Algorithms) {
            std::cout << " { id: " << algo.Id << ", name: " << algo.Name << " }";
        }
        std::cout << std::endl;

        auto paramInfo = ox->GetProfileAlgorithmParamsInfo( );
        std::cout << "ProfileAlgorithmParamsInfo: "
                    << " MinPeakHeightUnit[" << paramInfo.MinPeakHeightUnit << "]"
                    << " ThresholdUnit[" << paramInfo.ThresholdUnit << "]"
                    << " MinPeakWidthUnit[" << paramInfo.MinPeakWidthUnit << "]"
                    << std::endl;

        auto profileParameter = ox->GetProfileAlgorithmParameters( 0 );
        std::cout << "ProfileAlgorithmParameters: "
                    << " MinPeakHeight[" << profileParameter.MinPeakHeight << "]"
                    << " MinPeakWidth[" << profileParameter.MinPeakWidth << "]"
                    << " ThresholdValue[" << profileParameter.ThresholdValue << "]"
                    << " ThresholdType[" << profileParameter.ThresholdType << "]"
                    << std::endl;

        auto profileParametersInfo = ox->GetProfileAlgorithmParamsLimits( 0 );
        std::cout << "ProfileAlgorithmParameters: ";
        std::cout << " AlgorithmId[" << profileParametersInfo.AlgorithmId << "]";
        std::cout << " MinPeakHeight[" << std::endl;
        std::cout << "    Minimum: " << profileParametersInfo.Limit.MinPeakHeight.Minimum << std::endl;
        std::cout << "    Maximum: " << profileParametersInfo.Limit.MinPeakHeight.Maximum << std::endl;
        std::cout << "]" << std::endl;
        std::cout << " MinPeakWidth[" << std::endl;
        std::cout << "    Minimum: " << profileParametersInfo.Limit.MinPeakWidth.Minimum << std::endl;
        std::cout << "    Maximum: " << profileParametersInfo.Limit.MinPeakWidth.Maximum << std::endl;
        std::cout << "]" << std::endl;
        std::cout << " ThresholdValue[" << std::endl;
        std::cout << "    Minimum: " << profileParametersInfo.Limit.ThresholdValue.Minimum << std::endl;
        std::cout << "    Maximum: " << profileParametersInfo.Limit.ThresholdValue.Maximum << std::endl;
        std::cout << "]" << std::endl;
        std::cout << "ThresholdTypes: [" << std::endl;
        for(auto type : profileParametersInfo.Limit.ThresholdTypes) {
            std::cout << "   { TypeId: " << type.TypeId << ", name: " << type.Name << " }" << std::endl;
        }
        std::cout << "]" << std::endl;

        auto zaxis = ox->GetZAxis();
        std::cout << "ZAxis: " << zaxis << std::endl;

        auto axesInfo = ox->GetAxesInfo(  );
        std::cout << "AxesInfo: [" << std::endl;
        for(auto def : axesInfo.ZAxisDefinitions) {
            std::cout << "   { id: " << def.Id << ", name: " << def.Name << " }" << std::endl;
        }
        std::cout << "]" << std::endl;

        /* Field Of View */
        auto fovInfo = ox->GetFieldOfViewInfo(  );
        std::cout << "FieldOfViewInfo: "
                    << " XUnit[" << fovInfo.XUnit << "]"
                    << " ZUnit[" << fovInfo.ZUnit << "]"
                    << " XPrecision[" << fovInfo.XPrecision << "]"
                    << " ZPrecision[" << fovInfo.ZPrecision << "]"
                    << std::endl;

        auto fovLimits = ox->GetFieldOfViewLimits(  );
        std::cout << "FieldOfViewLimits: "
                    << " MaxXMinus[" << fovLimits.MaxXMinus << "]"
                    << " MaxXPlus[" << fovLimits.MaxXPlus << "]"
                    << " MinWidth[" << fovLimits.MinWidth << "]"
                    << " MinHeight[" << fovLimits.MinHeight << "]"
                    << " MaxHeight[" << fovLimits.MaxHeight << "]"
                    << std::endl;

        auto fov = ox->GetFieldOfView(  );
        std::cout << "FieldOfView: "
                    << " LimitLeft[" << fov.LimitLeft << "]"
                    << " LimitRight[" << fov.LimitRight << "]"
                    << " Offset[" << fov.Offset << "]"
                    << " Height[" << fov.Height << "]"
                    << std::endl;

        auto fovDistance = ox->GetFieldOfViewDistance(  );
        std::cout << "FieldOfViewDistance: "
                    << " LimitLeft[" << fovDistance.LimitLeft << "]"
                    << " LimitRight[" << fovDistance.LimitRight << "]"
                    << " Near[" << fovDistance.Near << "]"
                    << " Far[" << fovDistance.Far << "]"
                    << std::endl;

        ox->Logout( );

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // closes the network connection
    ox->Disconnect( );
}

void ExampleMeasurement(const std::string& host)
{
    // set output format
    std::cout.precision(3);
    std::cout << std::fixed;

    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host );

    try
    {
        // opens the network connection
        ox->Connect( );

        // get additional measurement information
        auto measurementInfo = ox->GetMeasurementInfo( );
        std::cout << "MeasurementInfo:" << std::endl;
        std::cout << "  TimeStampUnits: [";
        for( auto value : measurementInfo.TimeStampUnits )
        {
            std::cout << " " << value;
        }
        std::cout << " ]" << std::endl;
        std::cout << "  QualityValues: [" << std::endl;
        for( auto value : measurementInfo.QualityValues )
        {
            std::cout << "   { id: " << value.Id << ", name: " << value.Name << " }" << std::endl;
        }
        std::cout << "  ]" << std::endl;
        std::cout << "  MeasurementRateUnit[ " << measurementInfo.MeasurementRateUnit << " ]" << std::endl;
        std::cout << "  MeasurementRatePrecision[ " << measurementInfo.MeasurementRatePrecision << " ]" << std::endl;

        // get additional measurement values information
        auto measurementValuesInfo = ox->GetMeasurementValuesInfo();
        std::cout << "MeasurementTypes: [" << std::endl;
        for( auto type : measurementValuesInfo.MeasurementTypes )
        {
            std::cout << "   {";
            std::cout << " ToolId: " << type.ToolId;
            std::cout << ", Mode: " << type.Mode;
            std::cout << ", Tool: " << type.Tool;
            std::cout << ", Name: " << type.Name;
            std::cout << ", Unit: " << type.Unit;
            std::cout << ", Precision: " << type.Precision;
            std::cout << ", Minimum: " << type.Minimum;
            std::cout << ", Maximum: " << type.Maximum;
            std::cout <<  " }" << std::endl;
        }
        std::cout << "]" << std::endl;

        // get the latest measurement values
        auto measurement = ox->GetMeasurement();
        std::cout << "MeasurementValue:" << std::endl;
        std::cout << "  Quality[ " << measurement.Quality << " ]" << std::endl;
        std::cout << "  ConfigMode[ " << measurement.ConfigModeActive << " ]" << std::endl;
        std::cout << "  Alarm[ " << measurement.Alarm << " ]" << std::endl;
        std::cout << "  DigitalOuts: [";
        for( auto value : measurement.DigitalOuts )
        {
            std::cout << " " << value;
        }
        std::cout << " ]" << std::endl;
        std::cout << "  EncoderValue[ " << measurement.EncoderValue << " ]" << std::endl;
        std::cout << "  TimeStamp[ " << measurement.TimeStamp << " ]" << std::endl;
        std::cout << "  MeasurementRate[ " << measurement.MeasurementRate << " ]" << std::endl;
        std::cout << "  Values: [";
        for( auto value : measurement.Values )
        {
            std::cout << " " << value;
        }
        std::cout << " ]" << std::endl;

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // closes the network connection
    ox->Disconnect();
}

void ExampleProfile(const std::string& host)
{
    // set output format
    std::cout.precision(2);
    std::cout << std::fixed;

    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host );

    try
    {
        // opens the network connection
        ox->Connect();

        // get the latest profile
        auto profile = ox->GetProfile();
        std::cout << "Profile:" << std::endl;
        std::cout << "  QualityId[ " << profile.QualityId << " ]" << std::endl;
        std::cout << "  TimeStamp[ " << profile.TimeStamp << " ]" << std::endl;
        std::cout << "  Precision[ " << profile.Precision << " ]" << std::endl;
        std::cout << "  XStart[ " << profile.XStart << " ]" << std::endl;
        std::cout << "  Length[ " << profile.Length << " ]" << std::endl;
        std::cout << "  Points[";
        for (int i = 0; i < 5; i++)
        {
            std::cout << " {" << profile.X.at(i) << "," <<profile.Z.at(i) << "}";
        }
        std::cout << " ...";
        for (std::size_t i = profile.X.size() - 5; i < profile.X.size(); i++)
        {
            std::cout << " {" << profile.X.at(i) << "," <<profile.Z.at(i) << "}";
        }
        std::cout << " ]" << std::endl;

        // get additional profile information (e.g. x and z unit)
        auto profileInfo = ox->GetProfileInfo();
        std::cout << "ProfileInfo:" << std::endl;
        std::cout << "  MaxLength[ " << profileInfo.MaxLength << " ]" << std::endl;
        std::cout << "  XUnit[ " << profileInfo.XUnit << " ]" << std::endl;
        std::cout << "  ZUnit[ " << profileInfo.ZUnit << " ]" << std::endl;

        // the intensity profile is identical to the profile
        // but contains additional intensity values for each profile point
        auto intensityProfile = ox->GetIntensityProfile();
        std::cout << "IntensityProfile:" << std::endl;
        std::cout << "  QualityId[ " << intensityProfile.QualityId << " ]" << std::endl;
        std::cout << "  TimeStamp[ " << intensityProfile.TimeStamp << " ]" << std::endl;
        std::cout << "  Precision[ " << intensityProfile.Precision << " ]" << std::endl;
        std::cout << "  XStart[ " << intensityProfile.XStart << " ]" << std::endl;
        std::cout << "  Length[ " << intensityProfile.Length << " ]" << std::endl;
        std::cout << "  Points[";
        for (int i = 0; i < 5; i++)
        {
            std::cout << " {" << intensityProfile.X.at(i) << "," <<intensityProfile.Z.at(i) << "}";
        }
        std::cout << " ...";
        for (std::size_t i = intensityProfile.X.size() - 5; i < intensityProfile.X.size(); i++)
        {
            std::cout << " {" << intensityProfile.X.at(i) << "," <<intensityProfile.Z.at(i) << "}";
        }
        std::cout << " ]" << std::endl;
        std::cout << "  Points[";
        for (int i = 0; i < 5; i++)
        {
            std::cout << " " << intensityProfile.I.at(i);
        }
        std::cout << " ...";
        for (std::size_t i = intensityProfile.X.size() - 5; i < intensityProfile.X.size(); i++)
        {
            std::cout << " " << intensityProfile.I.at(i);
        }
        std::cout << " ]" << std::endl;

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // closes the network connection
    ox->Disconnect();
}

void ExampleImage(const std::string& host, const std::string& outputFile)
{
    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host );

    std::string filename = outputFile != "" ? outputFile : "image.pgm";

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
        ox->SaveImage( filename, image );

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // closes the network connection
    ox->Disconnect();
}

void ExampleSoftwareTrigger(const std::string& host)
{
    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host );

    try
    {
        // opens the network connection
        ox->Connect();

        // login to get access to all configuration parameters
        ox->Login("admin", "");

        auto triggerInfo = ox->GetTriggerInfo();

        auto softwareTriggerItem = std::find_if( triggerInfo.TriggerModes.begin( ),
                                                 triggerInfo.TriggerModes.end( ),
                                                 []( const Baumer::OXApi::Types::TriggerMode &mode )
                                                 {   return mode.Name == "software";} );

        if( softwareTriggerItem == triggerInfo.TriggerModes.end( ) )
        {
            std::cout << "SoftwareTrigger not supported!" << std::endl;
            return;
        }

        ox->ConfigureTrigger((unsigned int)softwareTriggerItem->Id, 0, 0, 0);

        for (int i = 0; i < 10; i++)
        {
            ox->Trigger(1);

            auto measurement = ox->GetMeasurement();

            std::cout << "Measurement TimeStamp: " << measurement.TimeStamp << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        auto freerunTriggerItem = std::find_if( triggerInfo.TriggerModes.begin( ),
                                                 triggerInfo.TriggerModes.end( ),
                                                 []( const Baumer::OXApi::Types::TriggerMode &mode )
                                                 {   return mode.Name == "freerun";} );

        if( freerunTriggerItem == triggerInfo.TriggerModes.end( ) )
        {
            std::cout << "FreeRunTrigger not supported!" << std::endl;
            return;
        }

        ox->ConfigureTrigger((unsigned int)freerunTriggerItem->Id, 0, 0, 0);

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what( ) << std::endl;
    }

    // closes the network connection
    ox->Disconnect();
}

void ExampleLoadStoreConfiguration(const std::string& host)
{
    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host );

    try
    {
        // opens the network connection
        ox->Connect( );

        // login to get access to all configuration parameters
        ox->Login( "admin" );

        auto num = ox->GetNumberOfSetups( );
        std::cout << "NumberOfSetups: " << num << std::endl;

        auto startup = ox->GetStartupSetup( );
        std::cout << "StartupSetup: " << startup << std::endl;

        auto active = ox->GetActiveSetup( );
        std::cout << "ActiveSetup: ";
        std::cout << " Number[" << active.Number << "]";
        std::cout << " Saved[" << active.Saved << "]";
        std::cout << std::endl;

        std::cout << "ExposureTime at beginning: " << ox->GetExposureTime( ) << std::endl;

        // store current configuration at parameter slot 1
        ox->StoreParameterSetup( 1 );

        ox->ConfigureExposureTime( 1000 );

        std::cout << "ExposureTime after change: " << ox->GetExposureTime( ) << std::endl;

        // load the previously stored configuration
        ox->LoadParameterSetup( 1 );

        std::cout << "ExposureTime after load: " << ox->GetExposureTime( ) << std::endl;

        // reads the configuration as json string
        auto json = ox->GetParameterSetup( 1 );
        std::cout << json << std::endl;

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what( ) << std::endl;
    }

    // closes the network connection
    ox->Disconnect();
}

void signal_handler(int signal)
{
    gSignalStatus = signal;
}

void StreamProfiles(const std::string& host, std::uint16_t udpPort, const std::string& outputFile)
{
    // install the signal handler for 'ctrl + c'
    std::signal(SIGINT, signal_handler);

    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host, udpPort );

    std::string filename = outputFile != "" ? outputFile : "profiles.txt";

    // create an instance of a OxStream object
    auto stream = ox->CreateStream();

    // set socket buffer size, needs to be set large enough so that no udp
    // packets get lost
    stream->SetReceiveBufferSize( 2 * 1024 * 1024 );

    try
    {
        std::size_t count{0};
        std::ofstream streamFile(filename);
        streamFile.precision(3);
        streamFile << std::fixed;

        // provide buffer for file writer stream
        // at least on windows this improves write performance
        const size_t bufsize = 1*1024*1024;
        auto buf = std::make_unique<char[]>(bufsize);
        streamFile.rdbuf()->pubsetbuf(buf.get(), bufsize);

        // start collecting data
        stream->Start();
        std::cout << "Listening on udp port " << udpPort << ".\n";
        std::cout << "Start collecting. Stop by pressing Ctrl + C." << std::endl;

        while( gSignalStatus == 0 )
        {
            if( stream->ProfileAvailable( ) )
            {
                auto profile = stream->ReadProfile( );
                ++count;
                for (std::size_t i = 0; i < profile.Length; i++)
                {
                    std::string timestamp(std::to_string(profile.TimeStamp));

                    // Converting numbers to string via std::to_string before writing
                    // them into the stream seems to be faster than using the '<<'
                    // operator overload directly.
                    streamFile  << timestamp << '\t' << std::to_string(profile.X.at(i));
                    if (profile.ZValid)
                    {
                        streamFile << '\t' << std::to_string(profile.Z.at(i));
                    }
                    if (profile.IntensityValid)
                    {
                        streamFile << '\t' << std::to_string(profile.I.at(i));
                    }
                    streamFile << "\n";
                }
                streamFile << "\n";
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            // check for errors
            if( stream->ErrorOccured( ) )
            {
                auto error = stream->ReadError( );
                std::cout << "Error BlockId: " << error.BlockId << " Message: " << error.Message
                        << std::endl;
            }
        }

        // stop collecting data
        stream->Stop();

        std::cout << "\nStop collecting (P: " << count << ")\n" << std::endl;

        streamFile.close();

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what( ) << std::endl;
    }

    // close stream
    stream->Close();
}

void StreamMeasurements(const std::string& host, std::uint16_t udpPort, const std::string& outputFile)
{
    // install the signal handler for 'ctrl + c'
    std::signal(SIGINT, signal_handler);

    // create an instance of a Ox object
    auto ox = Baumer::OXApi::Ox::Create( host, udpPort );

    std::string filename = outputFile != "" ? outputFile : "measurements.txt";

    // create an instance of a OxStream object
    auto stream = ox->CreateStream();

    // set socket buffer size, needs to be set large enough so that no udp
    // packets get lost
    stream->SetReceiveBufferSize( 2 * 1024 * 1024 );

    try
    {
        std::size_t count{0};
        std::ofstream streamFile(filename);
        streamFile.precision(3);
        streamFile << std::fixed;

        // start collecting data
        stream->Start();
        std::cout << "Listening on udp port " << udpPort << ".\n";
        std::cout << "Start collecting. Stop by pressing Ctrl + C." << std::endl;

        while( gSignalStatus == 0 )
        {
            // process data while collecting is not stopped
            if( stream->MeasurementAvailable( ) )
            {
                auto measurement = stream->ReadMeasurement( );
                ++count;
                streamFile << measurement.TimeStamp << '\t' << measurement.MeasurementRate << '\t'
                        << measurement.EncoderValue << '\t';
                for( auto value : measurement.Values )
                {
                    streamFile << value << '\t';
                }
                streamFile << '\n';
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            // check for errors
            if( stream->ErrorOccured( ) )
            {
                auto error = stream->ReadError( );
                std::cout << "Error BlockId: " << error.BlockId << " Message: " << error.Message
                        << '\n';
            }
        }

        // stop collecting data
        stream->Stop();

        std::cout << "\nStop collecting (M: " << count << ")\n" << std::endl;

        streamFile.close();

    } catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what( ) << std::endl;
    }

    // close stream
    stream->Close();
}

class ArgumentParser
{
public:
    ArgumentParser( int argumentsCount, char * arguments[] )
    {
        for(int i = 1; i < argumentsCount; ++i)
        {
            m_elements.push_back(std::string(arguments[i]));
        }
    }

    const std::string& GetOption(const std::string &option)
    {
        auto iter = std::find(m_elements.begin(), m_elements.end(), option);
        if(iter != m_elements.end() && ++iter != m_elements.end())
        {
            return *iter;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    bool OptionExists(const std::string &option)
    {
        auto iter = std::find(m_elements.begin(), m_elements.end(), option);
        return iter != m_elements.end();
    }

    const std::string& GetPostionalOption(std::size_t position)
    {
        return m_elements.at(position);
    }
private:
    std::vector<std::string> m_elements;
};

int main(int argc, char * argv[])
{
    const std::string help = 
        "\n"
        "Usage\n"
        "  oxapiexamples <example> [-s <sensor host>] [-u <udp port>] [-o <output file>] [-h]\n"
        "\n"
        "Options:\n"
        "  <example>         example no.\n"
        "    1                 read configuration\n"
        "    2                 read measurement\n"
        "    3                 read profile\n"
        "    4                 read image\n"
        "    5                 software trigger\n"
        "    6                 load/store configuration\n"
        "    7                 udp stream profiles\n"
        "    8                 udp stream measurements\n"
        "  -s <sensor host>  connect to sensor at <sensor host> (ip address or host name) [default: 192.168.0.250]\n"
        "  -u <udp port>     udp port to listen on [default: 12345]\n"
        "  -o <output file>  output file [default: depends on example]\n"
        "  -h                display this help\n"
        "\n";

    ArgumentParser arguments(argc, argv);

    if(arguments.OptionExists("-h"))
    {
        std::cout << help;
        return 0;
    }

    std::string host = arguments.GetOption("-s");
    if(host == "")
    {
        host = "192.168.0.250";
    }
    
    std::string outputFile = arguments.GetOption("-o");

    std::string udpPortString = arguments.GetOption("-u");
    if(udpPortString.empty())
    {
        udpPortString = "12345";
    }

    int example = 0;
    std::uint16_t udpPort = 0;
    try
    {
        example = std::stoi(arguments.GetPostionalOption(0));
        udpPort = std::stoi(udpPortString);
    }
    catch(...)
    {
        std::cout << help;
        return 1;
    }

    switch (example)
    {
    case 1:
        ExampleReadConfiguration(host);
        break;
    case 2:
        ExampleMeasurement(host);
        break;
    case 3:
        ExampleProfile(host);
        break;
    case 4:
        ExampleImage(host, outputFile);
        break;
    case 5:
        ExampleSoftwareTrigger(host);
        break;
    case 6:
        ExampleLoadStoreConfiguration(host);
        break;
    case 7:
        StreamProfiles(host, udpPort, outputFile);
        break;
    case 8:
        StreamMeasurements(host, udpPort, outputFile);
        break;
    default:
        std::cout << help;
        return 1;
        break;
    }
    return 0;
}
