# This class uses pythonnet https://pypi.org/project/pythonnet/ to load and use a .NET assembly.
# Please add pythonnet to you python installation:
#       pip install pythonnet==2.4.0
import clr
import sys, os

sys.path.append(os.path.dirname(__file__) + r"\\API")

# Check if OxApi.dll can be found in path
if(not clr.FindAssembly('OxApi')):
    print('Error: OxApi.dll could not be found!')
    print('Please add the OX SDK installation directory to the python path,')
    print('e.g. sys.path.append(r"C:\Program Files\Baumer\OXSDK")')
    raise


# Add a reference to the .NET OX SDK Library (OxApi.dll)
clr.AddReference('OxApi')

import System
# Import the required namespaces
from Baumer.OXApi import Ox


class oxstream:

    def __init__(self, client):
        self.client = client

    def Close(self):
        """ Closes the streaming client. """
        self.client.Close()

    def Start(self):
        """ Starts reading data from the streaming sensor. """
        self.client.Start()

    def Stop(self):
        """ Stops reading data from the streaming sensor. """
        self.client.Stop()

    def GetProfileCount(self):
        """  The number of queued profiles.  """
        return self.client.ProfileCount

    def ProfileAvailable(self):
        """  Returns true if at least one profile is available.  """
        return self.client.ProfileAvailable

    def ReadProfile(self):
        """
        Reads one profile from the queue (the profile will be removed from the queue).
        Throws an exception if the queue is empty.
        Returns:
        (int): BlockId
        (bool):  Configuration mode active
        (bool): TimeSynchronized
        (bool): Values valid
        (bool): Alarm active
        (int):  Quality Id
        (double):  Timestamp
        (int):  Length
        (int):  Encoder Value
        (int[]):  Profile X-Values
        (int[]):  Profile Z-Values (None if not in stream)
        (int[]):  Profile Intensity-Values (None if not in stream)
        """
        profile =  self.client.ReadProfile()
        x = list(profile.X)
        z = None
        i = None
        if(profile.Z is not None):
            z = list(profile.Z)
        if(profile.I is not None):
            i = list(profile.I)
        
        return profile.BlockId, profile.ConfigModeActive, profile.TimeSyncedByNtp, profile.ValuesValid, profile.Alarm, profile.Quality, profile.Timestamp, profile.Length, profile.EncoderValue, x, z, i


    def ClearProfileQueue(self):
        """ Clears the profile queue. """
        self.client.ClearProfileQueue()

    def GetMeasurementCount(self):
        """  The number of queued measurements.  """
        return self.client.MeasurementCount

    def MeasurementAvailable(self):
        """   Returns true if at least one measurement is available.  """
        return self.client.MeasurementAvailable

    def ReadMeasurement(self):
        """
        Reads one measurement from the queue (the measurement will be removed from the queue).
        Throws an exception if the queue is empty.
        Returns:
        (int): BlockId
        (bool):  Configuration mode active
        (int):  Timestamp
        (bool): TimeSynchronized
        (bool): Values valid
        (int):  Quality
        (int):  Alarm
        (bool[]): Outputs  
        (double): MeasurementRate in Hz
        (int):  Encoder Value
        (float[]):  Measurement Values
        """
        m =  self.client.ReadMeasurement()
        values = list(m.Values)
        outputs = list(m.DigitalOuts)
        return m.BlockId, m.ConfigModeActive, m.Timestamp, m.TimeSyncedByNtp, m.ValuesValid, m.Quality, m.Alarm, outputs, m.MeasurementRate, m.EncoderValue, values

    def ErrorOccured(self):
        """   Returns true if at least one error is occured.  """
        return self.client.ErrorOccured

    def ReadError(self):
        """
        Returns one error from the queue (the error will be removed from the queue).
        Throws an exception if the queue is empty.
        Returns:
        (int): BlockId
        (int): ErrorType
        (str): Message
        """
        err =  self.client.ReadError()
        return err.BlockId, err.ErrorType, err.Message

    def ClearMeasurementQueue(self):
        """ Clears the measurement queue. """
        self.client.ClearMeasurementQueue()

    def SetQueueSize(self, size):
        """ Sets the length of all queues.
        Parameters:
        Size (int): The queue size, default is 10000.
        """
        self.client.QueueSize = System.Int32(size)

    def GetQueueSize(self):
        """ Gets the length of all queues. """
        return self.client.QueueSize

    def SetReceiveBufferSize(self, size):
        """ Sets the size of the udp socket receive buffer
        Parameters:
        Size (int): The buffer size
        """
        self.client.ReceiveBufferSize = System.Int32(size)

    def GetReceiveBufferSize(self):
        """ Gets the size of the udp socket receive buffer. """
        return self.client.ReceiveBufferSize

    def SetFullQueueHandling(self, handling):
        """ Sets the full queue handling
        Parameters:
        handling (int): 0: drop oldest, 1: ignore newest
        """
        self.client.FullQueueHandling = int(handling)

    def GetFullQueueHandling(self):
        """ Gets the full queue handling. """
        return self.client.FullQueueHandling


class ox:
    def __init__(self, ip, streamingPort = 1234):
        self.ox = Ox.Create(ip, streamingPort)
        self.ip = ip
        self.stream = None
        self.client = None

    def CreateStream(self):
        """ Creates an object to access streaming data. """
        self.stream = self.ox.CreateStream()
        if self.client is None:
            self.client = oxstream(self.stream)
        return self.client

    def Connect(self):
        """ Establishes a connection to the sensor. """
        self.ox.Connect()

    def Disconnect(self):
        """ Closes the sensor connection. """
        self.ox.Disconnect()

    def Login(self, role="admin", password=""):
        """  Changes the user role for the current session.
        Parameters:
        role (string):  The requested role(Default: "admin").
        password (string):  The password for the requested role(Default: "").
        """
        self.ox.Login(role, password)
        
    def Logout(self):
        """  Logs out the user role for the current session.
        """
        self.ox.Login("observer", "")

        
    def ConfigureNetwork(self, useDhcp, staticIp, subnetmask, gateway):
        """ Configures the network settings.
        Parameters:
        useDhcp (bool):     Enables or disables DHCP
        staticIP (string):  The static ip address, e.g. 192.168.0.250
        subnetMask (string):The subnet mask, e.g. 255.255.255.0
        gateway (string):   The gateway address, e.g. 192.168.0.1
        """
        dhcp = System.Boolean(useDhcp)
        self.ox.ConfigureNetwork(dhcp, staticIp, subnetmask, gateway)

    def GetNetworkConfiguration(self):
        """ Reads the network settings.
        Returns:
        (bool): DHCP enabled or disabled
        (string): The static ip address, e.g. 192.168.0.250
        (string): The subnet mask, e.g. 255.255.255.0
        (string): The gateway address, e.g. 192.168.0.1
        """
        nwc = self.ox.GetNetworkConfiguration()
        return nwc.DhcpActive, nwc.IpAddress, nwc.SubnetMask, nwc.Gateway, nwc.MacAddress


    def GetNumberOfTimeServers(self):
        """  Reads the number of time servers supported by the sensor for NTP.
        Returns:
        (int): The maximum number of servers.
        """
        return self.ox.GetNumberOfTimeServers()

    def GetTimeServerConfiguration(self):
        """ Reads the NTP configuration. 
        Returns:
        (bool):  NTP enabled or disabled .
        (string[]):  An array containing the timer server ip addresses.
        """
        tsc = self.ox.GetTimeServerConfiguration()
        return tsc.Enabled, list(tsc.TimeServers)
        
    def ConfigureTimeServer(self, useNtp, timeServers):
        """  Configures the NTP servers. 
        Parameters:
        useNtp (bool):  Enables or disables NTP.
        timeServers (string[]):  An array containing the timer server ip addresses.
        """
        ntp = System.Boolean(useNtp)
        self.ox.ConfigureTimeServer(ntp, timeServers)

    def ConfigureProcessInterfaces(self, enableModbus, enableOpcUa, enableUdpStreaming, udpStreamingIp, udpStreamingPort, realtimeProtocol, IoLinkProcessDataLayout):
        """ Configures the sensors process interfaces.
        Parameters:
        (bool):  Enables or disables Modbus TCP server.
        (bool):  Enables or disables OPC UA server.
        (bool): Destination ip address for UDP streaming.
        (string): Destination ip address for UDP streaming
        (int):  Destination port for UDP  streaming.
        (int): Id of the realtime protocol (from ProcessInterfacesInfo)
        (int): IO-Link process data layout ID
        """
        modbus = System.Boolean(enableModbus)
        upcUa = System.Boolean(enableOpcUa)
        udp = System.Boolean(enableUdpStreaming)
        port = System.UInt32(udpStreamingPort)
        rtp = System.UInt32(realtimeProtocol)
        iol = System.UInt32(IoLinkProcessDataLayout)
        self.ox.ConfigureProcessInterfaces(modbus, upcUa, udp,  udpStreamingIp, port, rtp, iol)

    def GetProcessInterfaces(self):
        """ Reads the sensors process interfaces configuration.
        Returns:
        (bool):  Modbus enabled
        (bool):  OPC UA enabled
        (int):  Realtime protocol ID
        (bool):  UDP Streaming enabled
        (string):  UDP Streaming IP address
        (int):  UDP Streaming port
        (int): IO-Link process data layout ID
        """
        pis = self.ox.GetProcessInterfaces()
        return pis.ModbusEnabled, pis.OPCUAEnabled, pis.RealtimeProtocol, pis.UdpStreamingEnabled, pis.UdpStreamingIp, pis.UdpStreamingPort, pis.IoLinkProcessDataLayout

    def GetProcessInterfacesInfo(self):
        """ Reads the information about the available process interfaces.
        Returns:
        (dict<int,string>):RealtimeProtocols: An object containing a list with the protocols.
        (dict<int,string>):IoLinkProcessDataLayouts: An object containing a list with the IO-Link process data layouts.
        """
        infos = self.ox.GetProcessInterfacesInfo()
        rtp = {}
        for info in infos.RealtimeProtocols:
            rtp[info.Id] = info.Name
        iol = {}
        for info in infos.IoLinkProcessDataLayouts:
            iol[info.Id] = info.Name
        return rtp, iol

    def GetActiveUdpStreams(self):
        """ Reads the activated UDP streams.
        Returns:
        (int[]): An array containing the active stream ids.
        """
        streams = self.ox.GetActiveUdpStreams()
        return list(streams)

    def ConfigureUdpStreams(self, streams):
        """This function is deprecated, use ConfigureActiveUdpStreams() instead.
        Activates all the UDP streams associated with the passed ids. Diables all other udp streams.
        Parameters:
        (int[])An array containing the stream ids to activate.    
        """
        self.ox.ConfigureActiveUdpStreams(streams)

    def ConfigureActiveUdpStreams(self, streams):
        """Activates all the UDP streams associated with the passed ids. Diables all other udp streams.
        Parameters:
        (int[])An array containing the stream ids to activate.    
        """
        self.ox.ConfigureActiveUdpStreams(streams)

    def GetUdpStreamingInfo(self):
        """ Reads the available UDP streams.
        Returns:
        (dict<int,string>): A dictionary containing the supported streams with id as key and name as value.
        """
        streamInfo = self.ox.GetUdpStreamingInfo()
        dict = {}
        for stream in streamInfo.UdpStreams:
            dict[stream.Id] = stream.Name
        return dict

    def GetSensorInfo(self):
        """ Reads the common sensor information.
        Returns:
        (string):  Sensortype
        (string):  Vendorname
        (string):  Serialnumber
        (string):  AggregateVersion
        (string):  SoftwareVersion
        """
        si = self.ox.GetSensorInfo()
        return si.Type, si.VendorName, si.SerialNumber, si.AggregateVersion, si.SoftwareVersion

    def ConfigureExposureTime(self, exposureTime):
        """ Configures the exposure time.
        Parameters:
        exposureTime (int): The exposure time, time resolution is typically in us, but should be read using GetExposureTimeInfo().
        """
        exTime = System.UInt32(exposureTime)
        self.ox.ConfigureExposureTime(exTime)

    def GetExposureTime(self):
        """ Reads the exposure time.
        Returns:
        (int): The exposure time, time resolution is typically in us, but should be read using GetExposureTimeLimits().
        """
        return self.ox.GetExposureTime()

    def GetExposureTimeLimits(self):
        """Reads information about the exposure time limits.
        Returns:
        (int): Minimum Time
        (int): Maximum Time
        """
        info = self.ox.GetExposureTimeLimits()
        return info.Minimum, info.Maximum

    def GetExposureTimeResolution(self):
        """Reads the resolution of the exposure time used for SetExposureTime / GetExposureTime / GetExposureTimeInfo.
        Returns:
        (string) - Exposure time Resolution: Resolution of the exposure time, typically µs.
        """
        return self.ox.GetExposureTimeResolution()

    def ConfigureProfileFilter(self, movingAverageEnabled, movingAverageLength):
        """
        Configures profile moving average filter.
        Parameters:
        (bool) - True/False - on/off moving average profile filter
        (int) - length of moving average filter [3 ... 15]
        """

        enabled = System.Boolean(movingAverageEnabled)
        length = System.UInt32(movingAverageLength)
        self.ox.ConfigureProfileFilter(enabled, length)

    def GetProfileFilter(self):
        """Reads the actual profile filter configuration.
        Returns:
        (bool, int): Moving Average Enabled, Moving Average Length: An object containing the state and the length of the filter.
        """
        profileFilter = self.ox.GetProfileFilter()
        return profileFilter.Enabled, profileFilter.MovingAverageLength

    def IsProfileFilterEnabled(self):
        """Reads the current profile filter state.
        Returns:
        (bool): True if the profile filter is enabled, false otherwise.
        """
        return self.ox.IsProfileFilterEnabled()

    def GetProfileFilterLimits(self):
        """Reads the limits of the profile filter.
        Returns:
        (int): Minimum Length
        (int): Maximum Length
        """
        limits = self.ox.GetProfileFilterLimits()
        return limits.MinimumLength, limits.MaximumLength

    def ConfigureProfileAlgorithm(self, algorithmId):
        """Configures the algorithm used for profile calculation.
        Returns:
        (int): The id of the algorithm (ids provided by GetProfileAlgorithms)
        """
        self.ox.ConfigureProfileAlgorithm(System.UInt32(algorithmId))

    def GetProfileAlgorithm(self):
        """Reads the configured algorithm used for profile calculation.
        Returns:
        (int): AlgorithmId
        """
        return self.ox.GetProfileAlgorithm()

    def GetProfileAlgorithms(self):
        """Reads a list of supported profile algorithms.
        Returns:
        (dict<int,string>): A object containing the supported algorithms with id and name.
        """
        info = self.ox.GetProfileAlgorithms()
        dict = {}
        for algo in info.Algorithms:
            dict[algo.Id] = algo.Name
        return dict

    def ConfigureProfileAlgorithmParameters(self, algorithmId, minPeakHeight, thresholdValue, thresholdType, minPeakWidth):
        """
        Configures the parameters for a specific algorithm.
        Parameters:
        (int): algorithmId - The id of the algorithm to configure.
        (int): minPeakHeight - Minimum peak height.
        (int): thresholdValue - Threshold value.
        (int): thresholdType - Type of the threshold.
        (int): minPeakWidth - Minimum peak width.
        """
        algo = System.UInt32(algorithmId)
        mph = System.UInt32(minPeakHeight)
        tv = System.UInt32(thresholdValue)
        tt = System.UInt32(thresholdType)
        mpw = System.UInt32(minPeakWidth)
        self.ox.ConfigureProfileAlgorithmParameters(algo, mph, tv, tt, mpw)

    def ConfigureResolution(self, xresolution, zresolution):
        """ Configures the resolution.
        Parameters:
        (int) : X resolution
        (int) : Z resolution
        """
        x = System.UInt32(xresolution)
        z = System.UInt32(zresolution)
        self.ox.ConfigureResolution(x, z)

    def GetResolution(self):
        """ Reads the resolution.
        Returns:
        (int):  X resolution
        (int):  Z resolution
        """
        binning = self.ox.GetResolution()
        return binning.XResolution, binning.ZResolution

    def GetResolutionInfo(self):
        """Reads information about the available resolutions.
        Returns:
        (int[]):xrs: An object containing the available x resolutions. 
        (int[]):trs: An object containing the available z resolutions.
        """
        resolutionInfo = self.ox.GetResolutionInfo()
        xrs = list(resolutionInfo.XResolutions)
        trs = list(resolutionInfo.ZResolutions)
        return xrs, trs


    def ConfigureFieldOfView(self, limitLeft, limitRight, offset, height):
        """ Configures the field of view. 
        Parameters:
        (double) : limitLeft
        (double) : limitRight
        (double) : offset
        (double) : height
        """
        self.ox.ConfigureFieldOfView(float(limitLeft), float(limitRight), float(offset), float(height))

    def GetFieldOfView(self):
        """ Reads the field of view. 
        Returns:
        (double) : limitLeft
        (double) : limitRight
        (double) : offset
        (double) : height
        """
        fov = self.ox.GetFieldOfView()
        return fov.LimitLeft, fov.LimitRight, fov.Offset, fov.Height

    def ConfigureFieldOfViewDistance(self, limitLeft, limitRight, near, far):
        """ Configures the distance field of view. 
        Parameters:
        (double) : limitLeft
        (double) : limitRight
        (double) : near
        (double) : far
        """
        self.ox.ConfigureFieldOfViewDistance(float(limitLeft), float(limitRight), float(near), float(far))

    def GetFieldOfViewDistance(self):
        """ Reads the distance mode field of view. 
        Returns:
        (double) : limitLeft
        (double) : limitRight
        (double) : near
        (double) : far
        """
        fov = self.ox.GetFieldOfViewDistance()
        return fov.LimitLeft, fov.LimitRight, fov.Near, fov.Far

    def GetFieldOfViewLimits(self):
        """Reads the actual field of view limits from the sensor.
        Returns:
        (double): Max X Minus
        (double): Max X Plus
        (double): Min Width
        (double): Min Height
        (double): Max Height
        (double): Min Distance
        (double): Max Distance
        """
        limits = self.ox.GetFieldOfViewLimits()
        return limits.MaxXMinus, limits.MaxXPlus, limits.MinWidth, limits.MinHeight, limits.MaxHeight, limits.MinDistance, limits.MaxDistance

    def GetFieldOfViewInfo(self):
        """Reads information about the field of view.
        Returns:
        (string): X Unit
        (string): Z Unit
        (int): X Precision
        (int): Z Precision
        """
        info = self.ox.GetFieldOfViewInfo()
        return info.XUnit, info.ZUnit, info.XPrecision, info.ZPrecision

    def ConfigureResampling(self, enabled, gridValue):
        """Configures the resampling.
        Parameters:
        (bool): enabled: Enables or disabled the resampling
        (double): gridValue: The grid value used for resampling.
        """
        enable = System.Boolean(enabled)
        self.ox.ConfigureResampling(enable, float(gridValue))

    def IsResamplingEnabled(self):
        """Reads the current resampling configuration.
        Returns:
        (bool): True if resampling is enabled, false otherwise.
        """
        return self.ox.IsResamplingEnabled()

    def GetResamplingGridValue(self):
        """Reads the current resampling grid value.
        Returns:
        (bool, double): The resampling grid value, typically in mm, but GetResamplingInfo() should be used.
        """
        resamplingGrid = self.ox.GetResamplingGridValue()
        return resamplingGrid.Enabled, resamplingGrid.GridValue

    def GetResamplingInfo(self):
        """Reads resampling information from the sensor.
        Returns:
        (string): GridUnit
        (int): GridPrecision
        (double): MinimumGridValue
        (double): MaximumGridValue
        """
        info = self.ox.GetResamplingInfo()
        return info.GridUnit, info.GridPrecision, info.MinimumGridValue, info.MaximumGridValue

    def Trigger(self, count):
        """ Generates a software trigger. The profiles are acquired in free running mode.
        Parameters:
        Count (int): Number of trigger events.
        """
        self.ox.Trigger(count)

    def ConfigureTrigger(self, mode, option, time, encoderSteps):
        """ Configures the trigger
        Parameters:
        Trigger mode (int):
        Trigger option (int):
        Trigger time (int):
        Encoder value (int):
        """
        m = System.UInt32(mode)
        o = System.UInt32(option)
        t = System.UInt32(time)
        e = System.UInt32(encoderSteps)
        self.ox.ConfigureTrigger(m, o, t, e)

    def GetTrigger(self):
        """ Reads the trigger configuration
        Returns:
        (int): Trigger mode 
        (int): Trigger option 
        (int): Trigger time 
        (int): Trigger encoder steps
        """
        trigger = self.ox.GetTrigger()
        return trigger.Mode, trigger.Option, trigger.Time, trigger.EncoderSteps


    def GetTriggerInfo(self):
        """ Reads the trigger information.
        Returns:
        (string): The trigger time unit. 
        (dict<int,string>): The trigger modes.
        (dict<int,int[]>): The trigger available options for each mode.
        (dict<int,string>): The trigger options.
        """
        triggerInfo = self.ox.GetTriggerInfo()
        modes = {}
        for mode in triggerInfo.TriggerModes:
            modes[mode.Id] = mode.Name
        modeOptions = {}
        for mode in triggerInfo.TriggerModes:
            modeOptions[mode.Id] = list(mode.Options)
        options = {}
        for option in triggerInfo.TriggerOptions:
            options[option.Id] = option.Name

        return triggerInfo.TimeUnit, modes, modeOptions, options


    def GetTriggerLimits(self):
        """Reads the trigger limits.
        Returns:
        (int): Min Time: Minimum Time Intervall
        (int): Max Time: Maximum Time Intervall
        (int): Min Steps: Minimum Encoder steps
        (int): Max Steps: Maximum Encoder steps
        """
        trigger = self.ox.GetTriggerLimits()
        return trigger.MinTime, trigger.MaxTime, trigger.MinSteps, trigger.MaxSteps


    def GetMeasurement(self):
        """ Reads the measurement values.
        Returns:
        (int):  Quality
        (bool):  ConfigModeActive
        (bool):  Alarm
        (bool[]):  Digital Outs
        (int):  Encoder value
        (int):  Time stamp
        (double):  Measurement rate (Hz)
        (double[]):  Measurements (as defined in GetMeasurementInfo)
        """
        m = self.ox.GetMeasurement()
        values = list(m.Values)
        digitalOuts = list(m.DigitalOuts)
        return m.Quality, m.ConfigModeActive, m.Alarm, digitalOuts, m.EncoderValue, m.TimeStamp, m.MeasurementRate, values


    def GetProfileInfo(self):
        """ Reads the profile information.
        Returns:
        (int):  Maximum profile length in points.
        (string):  Unit of the x values.
        (string):  Unit of the u values.
        """
        info = self.ox.GetProfileInfo()
        return info.MaxLength, info.XUnit, info.ZUnit

    def GetProfile(self):
        """ Reads the profile.
        Returns:
        (int):  Quality Id
        (double):  Timestamp
        (int):  Precision
        (int):  X Start Value
        (int):  Length
        (int[]):  Profile X-Values
        (int[]):  Profile Z-Values
        """
        profile = self.ox.GetProfile()
        x = list(profile.X)
        z = list(profile.Z)
        return profile.Quality, profile.TimeStamp, profile.Precision, profile.XStart, profile.Length, x, z

    def GetIntensityProfile(self):
        """ Reads the profile.
        Returns:
        (int):  Quality Id
        (double):  Timestamp
        (int):  Precision
        (int):  X Start Value
        (int):  Length
        (int[]):  Profile X-Values
        (int[]):  Profile Z-Values
        (int[]):  Profile Intensity-Values
        """
        profile = self.ox.GetIntensityProfile()
        x = list(profile.X)
        z = list(profile.Z)
        i = list(profile.I)
        return profile.Quality, profile.TimeStamp, profile.Precision, profile.XStart, profile.Length, x, z, i

    def GetImageInfo(self):
        """ Reads the raw image information.
        Returns:
        (int):  Height of the sensor in pixels.
        (int):  Width of the sensor in pixels.
        (int):  Maximum pixel count of the ROI.
        """
        info = self.ox.GetImageInfo()
        return info.SensorHeight, info.SensorWidth, info.MaxROIPixels

    def GetImage(self):
        """ Reads the raw image.
        Returns:
        (int):  ROI height
        (int):  ROI width
        (int):  Row offset
        (int):  Column offset
        (int):  Row binning
        (int):  Column binning
        (int[]):  Pixels
        (lambda(filename)): A lambda function which saves the image to a file
        """
        image = self.ox.GetImage()
        pixels = list(image.Pixels)
        imageSaver = lambda f: image.Save(f)
        return image.RoiHeight, image.RoiWidth, image.RowOffset, image.ColumnOffset, image.RowBinning, image.ColumnBinning, pixels, imageSaver

    def LoadParameterSetup(self, storageNumber):
        """ Loads the parameter setup from the given storage.
        Parameters:
        (int):  The storage number.
        """
        self.ox.LoadParameterSetup(System.UInt32(storageNumber))

    def StoreParameterSetup(self, storageNumber):
        """ Stores the actual sensor configuration to the desired storage.
        Parameters:
        (int):  The storage number.
        """
        self.ox.StoreParameterSetup(System.UInt32(storageNumber))

    def GetActiveSetup(self):
        """ Read the current active setup.
        Returns:
        (int):  The storage number.
        (bool): Saved state.
        """
        setup = self.ox.GetActiveSetup()
        return setup.Number, setup.Saved

    def ConfigureStartupSetup(self, storageNumber):
        """ Configures which setup is loaded at sensor startup.
        Parameters:
        (int):  The storage number.
        """
        self.ox.ConfigureStartupSetup(System.UInt32(storageNumber))

    def GetStartupSetup(self):
        """ Returns which setup is loaded at startup.
        Returns:
        (int): The storage number of the setup.
        """
        return self.ox.GetStartupSetup()

    def GetParameterSetup(self, storageNumber):
        """ Reads the desired parameter setup and returns a json string.
        Parameters:
        (int): The storage number.
        Returns:
        (string):  The parameters as json string.
        """
        return self.ox.GetParameterSetup(System.UInt32(storageNumber))

    def GetNumberOfSetups(self):
        """ Reads the number of available setup storages.
        Returns:
        (int):  The number of available setup storages.
        """
        return self.ox.GetNumberOfSetups()

    def ReadAllSettings(self):
        """ Reads all settings from the sensor.
        Returns:
        (string):  An encoded string containing all settings.
        """
        return self.ox.ReadAllSettings()

    def ReadSetting(self, storageNumber):
        """ Reads a settings from the sensor.
        Parameters:
        (int): The storage number. "0" specifies device configuration parameters.
        Returns:
        (string):  An encoded string containing the setting.
        """
        return self.ox.ReadSetting(System.UInt32(storageNumber))

    def WriteAllSettings(self, settings):
        """  Writes all settings to the sensor.
        Parameters:
        (string):  The settings to write as encoded string (e.g. read by ReadAllSettings)
        """
        self.ox.WriteAllSettings(settings)
        
    def WriteSetting(self, setting, storageNumber):
        """  Writes a specific setting (parameter setup) to the sensor.
        Parameters:
            setting (string):  The setting to write as encoded string (e.g. read by ReadAllSettings)
            storageNumber (int): The storage number to import to (0 is device configuration)
        """
        self.ox.WriteSetting(setting, System.UInt32(storageNumber))

    def ResetSettings(self, storageNumber):
        """ Resets the settings for the desired storage.
        Parameters:
        (int): The storage number to reset.
        """            
        return self.ox.ResetSettings()

    def ResetAllSettings(self):
        """ Resets the settings for all storages.
        """            
        return self.ox.ResetAllSettings()

    def ConfigureLaserPower(self, factor):
        """ Configures the laser power.
        Parameters:
        (double): The laser power factor.
        """
        self.ox.ConfigureLaserPower(float(factor))

    def GetLaserPower(self):
        """ Reads the laser power.
        Returns:
        (double): The laser power factor.
        """
        return self.ox.GetLaserPower()

    def GetLaserPowerInfo(self):
        """ Reads the laser power information.
        Returns:
        (string):  The laser power factor unit.
        (double):  The laser power factor precision.
        """
        lpinfo = self.ox.GetLaserPowerInfo()
        return lpinfo.FactorUnit, lpinfo.FactorPrecision

    def GetLaserPowerLimits(self):
        """ Reads the laser power limits.
        Returns:
        (double):  The laser power minimum factor.
        (double):  The laser power maximum factor.
        (double[]):  The laser power predefined factors. 
        """
        limits = self.ox.GetLaserPowerLimits()
        PredefinedFactors = list(limits.PredefinedFactors)
        return limits.MinFactor, limits.MaxFactor, PredefinedFactors

    def GetMeasurementInfo(self):
        """ Reads information about the Measurements
        Returns:
        (string[]): timeStampUnits
        (dict<int,string>): qualityValues with Id as key and name as value. 
        (string): MeasurementRateUnit
        (int): MeasurementRatePrecision 
        """
        measurementInfo = self.ox.GetMeasurementInfo()
        timeStampUnits = list(measurementInfo.TimeStampUnits)
        qualityValues = {}
        for qv in measurementInfo.QualityValues:
            qualityValues[qv.Key] = qv.Value
        return timeStampUnits, qualityValues, measurementInfo.MeasurementRateUnit, measurementInfo.MeasurementRatePrecision

    def GetMeasurementValuesInfo(self):
        """Reads information about the measurement values.
        Returns: 
        A list of dictionaries with one for each configured measurement. 
        The following keys are available:
        (int): ToolId
        (string): Mode
        (string): Tool
        (string): Name
        (string): Unit
        (double): Precision
        (double): Minimum
        (double): Maximum
        """
        mvis = self.ox.GetMeasurementValuesInfo()
        measurementTypes = []
        for mvi in mvis.MeasurementTypes:
            measurement = {}
            measurement["ToolId"] = mvi.ToolId
            measurement["Mode"] = mvi.Mode
            measurement["Tool"] = mvi.Tool
            measurement["Name"] = mvi.Name
            measurement["Unit"] = mvi.Unit
            measurement["Precision"] = mvi.Precision
            measurement["Minimum"] = mvi.Minimum
            measurement["Maximum"] = mvi.Maximum
            measurementTypes.append(measurement)
        return measurementTypes

    def GetProfileAlgorithmParamsLimits(self, algorithmId):
        """ Reads information about the Profile Algorithm Parameters Limits and returns the available profile algorithm parameter limits for a specific algorithm.
        Parameters:
        (int): Algorithm Id.
        Returns:
        (int, int): MinPeakHeight: Minimum Limit, Maximum Limit.
        (int, int): ThresholdValueUnit: Minimum Limit, Maximum Limit.
        (int, int): MinPeakWidthUnit: Minimum Limit, Maximum Limit.
        (dict<int,string>): Threshold type Id and name.
        """
        papl = self.ox.GetProfileAlgorithmParamsLimits(algorithmId)
        limits = {}
        limits["MinPeakHeight"] = (papl.Limit.MinPeakHeight.Minimum, papl.Limit.MinPeakHeight.Maximum)
        limits["ThresholdValue"] = (papl.Limit.ThresholdValue.Minimum, papl.Limit.ThresholdValue.Maximum)
        limits["MinPeakWidth"] = (papl.Limit.MinPeakWidth.Minimum,papl.Limit.MinPeakWidth.Maximum)
        limits["ThresholdType"] = {}
        for tt in papl.Limit.ThresholdTypes:
            limits["ThresholdType"][tt.TypeId] = tt.Name
        return papl.AlgorithmId, limits

    def GetProfileAlgorithmParamsInfo(self):
        """ Reads information about the profile algorithm Pprameters Information and returns the units for the profile algorithm parameters.
        Returns:
        (string): Minimum peak height unit.
        (string): Threshold value  unit.
        (string): Minimum peak width unit.
        """
        papi = self.ox.GetProfileAlgorithmParamsInfo()
        return papi.MinPeakHeightUnit, papi.ThresholdUnit, papi.MinPeakWidthUnit

    def GetProfileAlgorithmParameters(self, algorithmId):
        """ Reads information about the Profile Algorithm Parameters for a specific profile computation algorithm.
        Parameters:
        (int): Algorithm Id.
        Returns:
        (string): MinPeakHeight: Minimum peak height.
        (string): ThresholdValue: Threshold value.
        (string): MinPeakWidth: Minimum peak width.
        (string): ThresholdType: Type of the threshold.
        """
        algorithmParams = self.ox.GetProfileAlgorithmParameters(algorithmId)
        return algorithmParams.MinPeakHeight, algorithmParams.ThresholdValue, algorithmParams.ThresholdType, algorithmParams.MinPeakWidth

    def GetAxesInfo(self):
        """ Reads information about the available axes.
        Parameters:
        (dict<int,string>): A dictionary with z-axis id as key and axis name as value.
        """
        axisInfos = self.ox.GetAxesInfo()
        ais = {}
        for axisInfo in axisInfos.ZAxisDefinitions:
            ais[axisInfo.Id] = axisInfo.Name
        return ais

    def ConfigureZAxis(self, zAxisID):
        """ Configures the z-axis.
        Parameters:
        (int): The z-axis id.
        """
        self.ox.ConfigureZAxis(System.UInt32(zAxisID))

    def GetZAxis(self):
        """ Reads the configred the z-axis.
        Returns:
        (int): The z-axis id.
        """
        return self.ox.GetZAxis()

    def GetSecondaryData(self):
        """ Reads the secondary data from the sensor
        Returns:
        (int): BootUpCounter: Number of times the sensor has been turned on.
        (int): OperationTime: Total operational time in minutes.
        (int): UpTime: Operation time since last boot up in minutes.
        (int): Temperature: Internal temperature of the sensor.
        (int): OperatingVoltage: Operating voltage of the sensor 
        """
        SecondaryData = self.ox.GetSecondaryData()
        return SecondaryData.BootUpCounter, SecondaryData.OperationTime, SecondaryData.UpTime, SecondaryData.Temperature, SecondaryData.OperatingVoltage
