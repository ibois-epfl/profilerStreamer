/*
 * OXApi/Ox.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_OX_H_
#define OXAPI_OX_H_

#ifdef _WIN32
#ifdef _EXPORTING
   #define CLASS_DECLSPEC    __declspec(dllexport)
#else
   #define CLASS_DECLSPEC    __declspec(dllimport)
#endif //_EXPORTING
#else //_WIN32
   #define CLASS_DECLSPEC
#endif //_WIN32

#include <OXApi/Types.h>
#include <OXApi/UdpStreaming/OxStream.h>

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

namespace Baumer {
namespace OXApi {

class CLASS_DECLSPEC Ox
{
public:
    virtual ~Ox() = default;

    /**
     * @brief  Creates a Ox object used to configure and measure.
     *         No connection will be established by this method.
     * @param[in]  host           The Ox host to connect to, e.g. 192.168.0.250 or oxm200.local
     * @param[in]  streamingPort  UDP Port for streaming, default 1234
     * @return The created instance.
     */
    static std::unique_ptr<Ox>
    Create( const std::string &host, const std::uint16_t &streamingPort = 1234 );

    /**
     * @brief  Create a streaming client to access profiles and measurements provided by UDP steaming.
     * @return The streaming client. Multiple calls return the same instance.
     */
    virtual std::shared_ptr<UdpStreaming::OxStream>
    CreateStream() = 0;

    /**
     * @brief  Establishes a connection to the sensor.
     */
    virtual void
    Connect() const = 0;

    /**
     * @brief  Closes the sensor connection.
     */
    virtual void
    Disconnect() const = 0;

    /**
     * @brief  Changes the user role for the current session.
     * @param[in]  role      The requested role.
     * @param[in]  password  The password for the requested role.
     */
    virtual void
    Login( const std::string &role, const std::string &password = "" ) const = 0;

    /**
     * @brief  Changes the user role back to 'observer' for the current session.
     */
    virtual void
    Logout() const = 0;

    /**
     * @brief  Reads the common sensor information.
     * @return An object containing common sensor information: type, serial number, etc.
     */
    virtual const Types::SensorInfo
    GetSensorInfo() const = 0;

    /**
     * @brief  Reads the actual network configuration.
     * @return An object containing DHCP state, static ip address, subnet mask, gateway address and mac address.
     */
    virtual const Types::NetworkConfiguration
    GetNetworkConfiguration() const = 0;

    /**
     * @brief  Configures the network settings.
     * @param[in]  useDhcp      Enables or disables DHCP.
     * @param[in]  staticIP     The static ip address, e.g. 192.168.0.250
     * @param[in]  subnetMask   The subnet mask, e.g. 255.255.255.0
     * @param[in]  gateway      The gateway address, e.g. 192.168.0.1
     */
    virtual void
    ConfigureNetwork( const bool &useDhcp, const std::string &staticIP,
                      const std::string &subnetMask, const std::string &gateway ) const = 0;

    /**
     * @brief  Configures the network settings.
     * @param[in]  netConfig   An object containing DHCP state, static ip address, subnet mask, and gateway address.
     */
    virtual void
    ConfigureNetwork( const Types::NetworkConfiguration &netConfig ) const = 0;

    /**
     * @brief  Reads the maximum number of time servers for NTP.
     * @return The maximum number of servers.
     */
    virtual std::uint32_t
    GetNumberOfTimeServers() const = 0;

    /**
     * @brief  Reads the current NTP time server configuration.
     * @return An object containing the NTP state and the time servers.
     */
    virtual const Types::TimeServerInfo
    GetTimeServerConfiguration() const = 0;

    /**
     * @brief  Configures the NTP servers.
     * @param[in]  useNTP       The new NTP state.
     * @param[in]  timeServers  The time servers.
     */
    virtual void
    ConfigureTimeServer( const bool &useNTP,
                         const std::vector<std::string> &timeServers ) const = 0;

    /**
     * @brief  Configures the NTP servers.
     * @param[in]  timeServerConfiguration   An object containing the NTP state and the time servers..
     */
    virtual void
    ConfigureTimeServer( const Types::TimeServerInfo &timeServerConfiguration ) const = 0;

    /**
     * @brief  Returns information about the available realtime protocols.
     * @return An object containing an list with the protocols.
     */
    virtual const Types::ProcessInterfacesInfo
    GetProcessInterfacesInfo() const = 0;

    /**
     * @brief  Reads the sensors process interfaces configuration.
     * @return An object containing the process interface configuration.
     */
    virtual const Types::ProcessInterfaces
    GetProcessInterfaces() const = 0;

    /**
     * @brief  Configures the sensors process interfaces.
     * @param[in]  enableModbus                Enables or disables Modbus TCP server.
     * @param[in]  enableOpcUa                 Enables or disables OPC UA server.
     * @param[in]  realtimeProtocolId          Id of the realtime protocol (from ProcessInterfacesInfo)
     * @param[in]  enableUdpStreaming          Enables or disables UDP streaming.
     * @param[in]  udpDestinationIp            Destination ip address for UDP streaming.
     * @param[in]  udpDestinationPort          Destination port for UDP  streaming.
     * @param[in]  ioLinkProcessDataLayoutId   Id of the IO-Link process data layout (from ProcessInterfacesInfo)
     */
    virtual void
    ConfigureProcessInterfaces( const bool &enableModbus, const bool &enableOpcUa,
                                const std::uint32_t &realtimeProtocolId,
                                const bool &enableUdpStreaming, const std::string &udpDestinationIp,
                                const std::uint32_t &udpDestinationPort,
                                const unsigned int &ioLinkProcessDataLayoutId ) const = 0;

    /**
     * @brief  Configures the sensors process interfaces.
     * @param[in]  processInterfacesConfiguration  An object containing the process interface configuration.
     */
    virtual void
    ConfigureProcessInterfaces(
                const Types::ProcessInterfaces &processInterfacesConfiguration ) const = 0;

    /**
     * @brief  Gets the available UDP streams.
     * @return An object containing the supported streams with id and name.
     */
    virtual const Types::UdpStreamInfo
    GetUdpStreamingInfo() const = 0;

    /**
     * @brief  Reads the activated UDP streams.
     * @return An container with the active stream ids.
     */
    virtual const std::vector<std::uint32_t>
    GetActiveUdpStreams() const = 0;

    /**
     * @brief  Configures the activated UDP streams.
     * @param[in]  streamIds  An container with the stream ids to activate.
     */
    virtual void
    ConfigureActiveUdpStreams( const std::vector<std::uint32_t> &streamIds ) const = 0;

    /**
     * @brief  Reads the actual laser power.
     * @return The laser power factor.
     */
    virtual double
    GetLaserPower() const = 0;

    /**
     * @brief  Configures the laser power.
     * @param[in]  factor  The laser power factor.
     */
    virtual void
    ConfigureLaserPower( const double &factor ) const = 0;

    /**
     * @brief  Reads the information about the laser power configurations.
     * @return An object containing the laser power precision and unit.
     */
    virtual const Types::LaserPowerInfo
    GetLaserPowerInfo() const = 0;

    /**
     * @brief  Reads the laser power limits.
     * @return An object containing the laser power limits and predefined values.
     */
    virtual const Types::LaserPowerLimits
    GetLaserPowerLimits() const = 0;

    /**
     * @brief  Reads the exposure time.
     * @return Exposure time, the resolution is typically in µs, but should be read using GetExposureTimeInfo()
     */
    virtual std::uint32_t
    GetExposureTime() const = 0;

    /**
     * @brief  Sets the exposure time.
     * @param[in]  exposureTime  Time resolution is typically in µs, but should be read using GetExposureTimeInfo()
     */
    virtual void
    ConfigureExposureTime( const std::uint32_t &exposureTime ) const = 0;

    /**
     * @brief  Reads information about the exposure time limits.
     * @return An object containing the minimum and maximum exposure time value
     */
    virtual const Types::ExposureTimeLimits
    GetExposureTimeLimits() const = 0;

    /**
     * @brief  Reads the resolution of the exposure time used for SetExposureTime / GetExposureTime / GetExposureTimeInfo.
     * @return Resolution of the exposure time, typically µs.
     */
    virtual const Types::ExposureTimeInfo
    GetExposureTimeInfo() const = 0;

    /**
     * @brief  Generates a software trigger. The profiles are acquired in free running mode.
     * @param[in]  count  Number of trigger events.
     */
    virtual void
    Trigger( const std::uint32_t &count ) const = 0;

    /**
     * @brief  Reads the trigger configuration.
     * @return An object containing the trigger mode, options and time.
     */
    virtual const Types::Trigger
    GetTrigger() const = 0;

    /**
     * @brief  Configures the trigger.
     * @param[in]  triggerMode   Trigger mode, use GetTriggerInfo for available modes.
     * @param[in]  modeOption    Trigger option, use GetTriggerInfo for available options.
     * @param[in]  triggerTime   Trigger time, use GetTriggerInfo for time unit.
     * @param[in]  encoderSteps  Trigger encoder steps
     */
    virtual void
    ConfigureTrigger( const std::uint32_t &triggerMode, const std::uint32_t &modeOption,
                      const std::uint32_t &triggerTime, const std::uint32_t &encoderSteps ) const = 0;

    /**
     * @brief  Configures the trigger.
     * @param[in]  triggerConfiguration  An object containing the trigger mode, options and time.
     */
    virtual void
    ConfigureTrigger( const Types::Trigger &triggerConfiguration ) const = 0;

    /**
     * @brief  Reads the trigger limits.
     * @return An object containing the minimum and maximum trigger time.
     */
    virtual const Types::TriggerLimits
    GetTriggerLimits() const = 0;

    /**
     * @brief  Reads the information about possible trigger configurations.
     * @return An object containing the available trigger modes and options.
     */
    virtual const Types::TriggerInfo
    GetTriggerInfo() const = 0;

    /**
     * @brief  Reads the resolution configuration.
     * @return An object containing x and z resolution.
     */
    virtual const Types::Resolution
    GetResolution() const = 0;

    /**
     * @brief  Configures the resolution.
     * @param[in]  xResolution  The x resolution, get possible values by GetBinningInfo.
     * @param[in]  zResolution  The z resolution, get possible values by GetBinningInfo.
     */
    virtual void
    ConfigureResolution( const std::uint32_t &xResolution,
                         const std::uint32_t &zResolution ) const = 0;

    /**
     * @brief  Configures the resolution.
     * @param[in]  resolution  An object containing x and z resolution.
     */
    virtual void
    ConfigureResolution( const Types::Resolution &resolution ) const = 0;

    /**
     * @brief  Reads information about the available resolutions.
     * @return An object containing the available x and z resolutions
     */
    virtual const Types::ResolutionInfo
    GetResolutionInfo() const = 0;

    /**
     * @brief  Reads the current resampling configuration.
     * @return True if resampling is enabled, false otherwise.
     */
    virtual bool
    IsResamplingEnabled() const = 0;

    /**
     * @brief  Reads the current resampling grid value.
     * @return An object containing the resampling grid value and the resampling state.
     */
    virtual const Types::ResamplingGrid
    GetResamplingGridValue() const = 0;

    /**
     * @brief  Configures the resampling.
     * @param[in]  enabled    Enables or disabled the resampling.
     * @param[in]  gridValue  The grid value used for resampling.
     */
    virtual void
    ConfigureResampling( const bool &enabled, const double &gridValue ) const = 0;

    /**
     * @brief  Configures the resampling.
     * @param[in]  resampling   An object containing the resampling grid value and the resampling state.
     */
    virtual void
    ConfigureResampling( const Types::ResamplingGrid &resampling ) const = 0;

    /**
     * @brief  Reads resampling information from the sensor.
     * @return An object containing the grid unit, grid precision, minimum and maximum grid value.
     */
    virtual const Types::ResamplingInfo
    GetResamplingInfo() const = 0;

    /**
     * @brief  Reads the actual profile filter configuration.
     * @return An object containing the state and the length of the filter.
     */
    virtual const Types::ProfileFilter
    GetProfileFilter() const = 0;

    /**
     * @brief  Configures the profile filter.
     * @param[in]  movingAverageEnabled   Enables or disables the profile filter.
     * @param[in]  movingAverageLength    Length of the moving average.
     */
    virtual void
    ConfigureProfileFilter( const bool &movingAverageEnabled,
                            const std::uint32_t &movingAverageLength ) const = 0;

    /**
     * @brief  Configures the profile filter.
     * @param[in]  profileFilter   An object containing the profile filter settings.
     */
    virtual void
    ConfigureProfileFilter( const Types::ProfileFilter &profileFilter ) const = 0;

    /**
     * @brief  Reads the current profile filter state.
     * @return True if the profile filter is enabled, false otherwise.
     */
    virtual bool
    IsProfileFilterEnabled() const = 0;

    /**
     * @brief  Returns the limits of the profile filter.
     * @return An object containing the minimum and maximum filter length.
     */
    virtual const Types::ProfileFilterLimits
    GetProfileFilterLimits() const = 0;

    /**
     * @brief  Gets the configured algorithm used for profile calculation.
     * @return The configured algorithm.
     */
    virtual std::uint32_t
    GetProfileAlgorithm() const = 0;

    /**
     * @brief  Configures the algorithm used for profile calculation.
     * @param[in]  algorithmId   The id of the algorithm (ids provided by GetProfileAlgorithms)
     */
    virtual void
    ConfigureProfileAlgorithm( const std::uint32_t &algorithmId ) const = 0;

    /**
     * @brief  Reads a list of supported profile algorithms.
     * @return A object containing the supported algorithms with id and name.
     */
    virtual const Types::ProfileAlgorithmInfo
    GetProfileAlgorithms() const = 0;

    /**
     * @brief  Returns the available profile algorithm parameter limits for a specific algorithm.
     * @param[in]  algorithmId   The algorithm id.
     * @return An object with limits for the specific algorithm.
     */
    virtual const Types::ProfileAlgorithmParamsLimits
    GetProfileAlgorithmParamsLimits( const std::uint32_t &algorithmId ) const = 0;

    /**
     * @brief  Returns the units for the profile algorithm parameters.
     * @return An object with the units for the algorithm parameters.
     */
    virtual const Types::ProfileAlgorithmParamsInfo
    GetProfileAlgorithmParamsInfo() const = 0;

    /**
     * @brief  Gets the actual configuration of a specific algorithm.
     * @param[in]  algorithmId   The id of the algorithm.
     * @return An object containing the algorithm parameters.
     */
    virtual const Types::ProfileAlgorithmParameters
    GetProfileAlgorithmParameters( const std::uint32_t &algorithmId ) const = 0;

    /**
     * @brief  Configures the parameters for a specific algorithm.
     * @param[in]  algorithmId     The id of the algorithm to configure.
     * @param[in]  minPeakHeight   Minimum peak height.
     * @param[in]  minPeakWidth    Minimum peak width.
     * @param[in]  thresholdValue  Threshold value.
     * @param[in]  thresholdType   Type of the threshold.
     */
    virtual void
    ConfigureProfileAlgorithmParameters( const std::uint32_t &algorithmId,
                                         const std::uint32_t &minPeakHeight,
                                         const std::uint32_t &minPeakWidth,
                                         const std::uint32_t &thresholdValue,
                                         const std::uint32_t &thresholdType ) const = 0;

    /**
     * @brief  Configures the parameters for a specific algorithm.
     * @param[in]  algorithmId  The id of the algorithm to configure.
     * @param[in]  parameters   An object containing the algorithm parameters.
     */
    virtual void
    ConfigureProfileAlgorithmParameters(
                const std::uint32_t &algorithmId,
                const Types::ProfileAlgorithmParameters &parameters ) const = 0;

    /**
     * @brief  Reads information about the z-axes.
     * @return An object containing axis information.
     */
    virtual const Types::AxesInfo
    GetAxesInfo() const = 0;

    /**
     * @brief  Reads the current z-axis.
     * @return The Id of the current z-axis.
     */
    virtual std::uint32_t
    GetZAxis() const = 0;

    /**
     * @brief  Selects the z-axis.
     * @param[in]  zAxisId   The Id of the desired z-axis.
     */
    virtual void
    ConfigureZAxis( const std::uint32_t &zAxisId ) const = 0;

    /**
     * @brief  Reads the actual field of view settings from the sensor.
     * @return An object containing the field of view settings.
     */
    virtual const Types::FieldOfView
    GetFieldOfView() const = 0;

    /**
     * @brief  Configures the field of view.
     * @param[in]  limitLeft  The left limit.
     * @param[in]  limitRight The right limit.
     * @param[in]  offset     The offset.
     * @param[in]  height     The height.
     */
    virtual void
    ConfigureFieldOfView( const double &limitLeft, const double &limitRight, const double &offset,
                          const double &height ) const = 0;

    /**
     * @brief  Configures the field of view.
     * @param[in]  fieldOfView   An object containing the field of view configuration.
     */
    virtual void
    ConfigureFieldOfView( const Types::FieldOfView &fieldOfView ) const = 0;

    /**
     * @brief  Reads the actual field of view distance settings from the sensor.
     * @return An object containing the field of view distance settings.
     */
    virtual const Types::FieldOfViewDistance
    GetFieldOfViewDistance() const = 0;

    /**
     * @brief  Configures the field of view distance.
     * @param[in]  limitLeft  The left limit.
     * @param[in]  limitRight The right limit.
     * @param[in]  near       The near distance.
     * @param[in]  far        The far distance.
     */
    virtual void
    ConfigureFieldOfViewDistance( const double &limitLeft, const double &limitRight,
                                  const double &near, const double &far ) const = 0;

    /**
     * @brief  Configures the field of view distance.
     * @param[in]  fieldOfViewDistance   An object containing the field of view distance configuration.
     */
    virtual void
    ConfigureFieldOfViewDistance( const Types::FieldOfViewDistance &fieldOfViewDistance ) const = 0;

    /**
     * @brief  Reads the actual field of view limits from the sensor.
     * @return An object containing the field of view limits.
     */
    virtual const Types::FieldOfViewLimits
    GetFieldOfViewLimits() const = 0;

    /**
     * @brief  Reads information about the field of view.
     * @return An object containing the field of view X and Z precision and unit..
     */
    virtual const Types::FieldOfViewInfo
    GetFieldOfViewInfo() const = 0;

    /**
     * @brief  Reads all settings from the sensor.
     * @return An encoded string containing all settings.
     */
    virtual const std::string
    ReadAllSettings() const = 0;

    /**
    * @brief Reads a setting from the sensor.
    * @param[in] number The setting to read, 0 reads the device configuration
    * @return An encoded string containing one setting.
    */
    virtual const std::string
    ReadSetting( const unsigned int &number ) const = 0;

    /**
     * @brief  Writes all settings to the sensor.
     * @param[in]  settings   The settings to write as encoded string (e.g. read by ReadAllSettings)
     */
    virtual void
    WriteAllSettings( const std::string &settings ) const = 0;

    /**
     * @brief Writes a setting to the sensor
     * @param[in] setting The setting to write as an encoded string (e.g. read by ReadSetting)
     * @param[in] number The setting number where to store the setting. 0 writes the device configuration.
     */
    virtual void
    WriteSetting( const std::string &setting, const unsigned int &number ) const = 0;

    /**
     * @brief  Returns which setup is loaded at startup.
     * @return The storage number of the setup.
     */
    virtual std::uint32_t
    GetStartupSetup() const = 0;

    /**
     * @brief  Configures which setup is loaded at sensor startup.
     * @param[in]  storageNumber   The storage number.
     */
    virtual void
    ConfigureStartupSetup( const std::uint32_t &storageNumber ) const = 0;

    /**
     * @brief  Loads the parameter setup from the given storage.
     * @param[in]  storageNumber   The storage number.
     */
    virtual void
    LoadParameterSetup( const std::uint32_t &storageNumber ) const = 0;

    /**
     * @brief  Stores the actual sensor configuration to the desired storage.
     * @param[in]  storageNumber   The storage number.
     */
    virtual void
    StoreParameterSetup( const std::uint32_t &storageNumber ) const = 0;

    /**
     * @brief  Resets the settings for all storages.
     */
    virtual void
    ResetAllSettings() const = 0;

    /**
     * @brief  Reads the desired parameter setup and returns a json string.
     * @param[in]  storageNumber   The storage number.
     * @return The parameters as json string.
     */
    virtual const std::string
    GetParameterSetup( const std::uint32_t &storageNumber ) const = 0;

    /**
     * @brief  Reads the number of available setup storages.
     * @return The number of available setup storages
     */
    virtual std::uint32_t
    GetNumberOfSetups() const = 0;

    /**
     * @brief  Read the current active setup.
     * @return An object containing the setup number and the saved state.
     */
    virtual const Types::ActiveSetup
    GetActiveSetup() const = 0;

    /**
     * @brief  Reads the last measurements from the sensor.
     * @return An object containing all measurement values plus meta information.
     */
    virtual const Types::MeasurementValue
    GetMeasurement() const = 0;

    /**
     * @brief  Reads information about the measurement.
     * @return An object containing information about the measurement.
     */
    virtual const Types::MeasurementInfo
    GetMeasurementInfo() const = 0;

    /**
     * @brief  Reads information about the measurement values.
     * @return An object containing information about the measurement values.
     */
    virtual const Types::MeasurementValuesInfo
    GetMeasurementValuesInfo() const = 0;

    /**
     * @brief  Reads the last measured profile from the sensor.
     * @return An object containing a profile and additional meta information.
     */
    virtual const Types::Profile
    GetProfile() const = 0;

    /**
     * @brief  Returns information about the measured profile.
     * @return An object containing he maximum length in points, x and z axes units of the profile.
     */
    virtual const Types::ProfileInfo
    GetProfileInfo() const = 0;

    /**
     * @brief  Reads the last measured intensity profile from the sensor.
     * @return An object containing an profile with additional intensity values plus meta information.
     */
    virtual const Types::IntensityProfile
    GetIntensityProfile() const = 0;

    /**
     * @brief  Reads the last measured raw image from the sensor.
     * @return An object containing an image as pixel array and additional meta information.
     */
    virtual const Types::Image
    GetImage() const = 0;

    /**
     * @brief  Returns information about the measured image.
     * @return An object containing he maximum width and height in pixels and the maximum number of pixels in the region of interest.
     */
    virtual const Types::ImageInfo
    GetImageInfo() const = 0;

    /**
     * @brief  Saves the given image object to a file in Portable Graymap (PGM) Format.
     * On an Ubuntu system, the PMG file can convertg to PNG with the command:
     *    $ convert image.pgm image.png
     * @param[in]  filename Filename with type '.pgm'.
     * @param[in]  image    The image object read by GetImage().
     */
    virtual void
    SaveImage( const std::string &filename, const Types::Image &image ) const = 0;
};

} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_OX_H_ */
