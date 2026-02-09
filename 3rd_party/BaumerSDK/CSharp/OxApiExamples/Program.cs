using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Baumer.OXApi;
using Baumer.OXApi.UdpStreaming;

namespace OxApiExamples
{
	class Program
	{
		static void Main(string[] args)
		{
			// uncomment the method call to run the desired example

			//Writes all configurable parameter options of the sensor to a file.
			GenerateAllParamOptions();

			// ########################################################################
			// these examples show the usage of the OX configuration api
			// please notice the following points:
			// - if the sensor's webinterface is open, the API cannot connect (concurrent configuration connections are not supported)
			// - having an configuration session open may slow down the measurement rate
			// ########################################################################

			//ExampleReadConfiguration();

			// ExampleMeasurement();

			// ExampleProfile();

			// ExampleImage();

			// ExampleSoftwareTrigger();

			// ExampleLoadStoreConfiguration();


			// ########################################################################
			// these examples show the usage of the OX streaming api
			// if no data is acquired please check the following points:
			// - is the sensor trigger configured correctly
			// - may the windows firewall block receiving data on the configured port
			// ########################################################################

			// StreamProfiles();

			// StreamMeasurements();

		}
		private static void GenerateAllParamOptions()
		{
			// create an instance of a Ox object
			var ox = Ox.Create("192.168.0.250");

			// opens the network connection
			ox.Connect();

			// login to get access to all configuration parameters
			ox.Login("admin", "");

			//Change the location and name of the file as you want.
			StreamWriter streamWriter = new StreamWriter("ConfigurableParamOptions.md");

			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetTriggerInfo'></a>");
			var trigInfo = ox.GetTriggerInfo();
			streamWriter.WriteLine("| Mode Id | Mode Name | Supported Options |");
			streamWriter.WriteLine("| ------- | --------- | ----------------- |");
			foreach (var trig in trigInfo.TriggerModes)
			{
				streamWriter.Write($"| {trig.Id} | {trig.Name} | ");
				foreach (var opt in trig.Options)
				{
					streamWriter.Write($"{opt}");
				}
				streamWriter.WriteLine(" |");
			}
			streamWriter.WriteLine();

			streamWriter.WriteLine("| Option Id | Option Name |");
			streamWriter.WriteLine("| --------- | ----------- |");
			foreach (var trig in trigInfo.TriggerOptions)
			{
				streamWriter.WriteLine($"| {trig.Id} | {trig.Name} |");
			}
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetTriggerLimits'></a>");
			var trigLimits = ox.GetTriggerLimits();
			streamWriter.WriteLine("|  Property | Value |");
			streamWriter.WriteLine("| - | - |");
			streamWriter.WriteLine("| **Max Encoder Steps** |" + $"{trigLimits.MaxSteps} |");
			streamWriter.WriteLine("| **Min Encoder Steps** |" + $"{trigLimits.MinSteps} |");
			streamWriter.WriteLine("| **Max Interval Time** |" + $"{trigLimits.MaxTime} |");
			streamWriter.WriteLine("| **Min Interval Time** |" + $"{trigLimits.MinTime} |");
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetProcessInterfacesInfo'></a>");
			var procInt = ox.GetProcessInterfacesInfo();
			streamWriter.WriteLine("| Process Id | Process Name |");
			streamWriter.WriteLine("| - | - |");
			foreach (var pint in procInt.RealtimeProtocols)
			{
				streamWriter.WriteLine($"| {pint.Id} | {pint.Name} |");
			}
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetUdpStreamingInfo'></a>");
			var udpStream = ox.GetUdpStreamingInfo();
			streamWriter.WriteLine("| UDP Stream Id | UDP Stream Name| ");
			streamWriter.WriteLine("| - | - |");
			foreach (var ustream in udpStream.UdpStreams)
			{
				streamWriter.WriteLine($"| {ustream.Id} | {ustream.Name} |");
			}
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetProfileAlgorithms'></a>");
			var profileAlgo = ox.GetProfileAlgorithms();
			streamWriter.WriteLine("| Algorithm Id | Algorithm Name |");
			streamWriter.WriteLine("| - | - |");
			foreach (var algo in profileAlgo.Algorithms)
			{
				streamWriter.WriteLine($"| {algo.Id} | {algo.Name} |");

			}
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetProfileAlgorithmParamsLimits-System-Int32-'></a>");

			var profileLimits = ox.GetProfileAlgorithmParamsLimits(profileAlgo.Algorithms[0].Id).Limit;
			streamWriter.WriteLine($"|Algorithm Name: {profileAlgo.Algorithms[0].Name} | Algorithm Id: {profileAlgo.Algorithms[0].Id}|");
			streamWriter.WriteLine("| - | - |");
			streamWriter.WriteLine("| **Min Peak Height Range** | " + $"{profileLimits.MinPeakHeight.Minimum} - {profileLimits.MinPeakHeight.Maximum} |");
			streamWriter.WriteLine("| **Min Peak Width Range** | " + $"{profileLimits.MinPeakWidth.Minimum} - {profileLimits.MinPeakWidth.Maximum} |");
			streamWriter.WriteLine("| **Threshold Value Range** | " + $"{profileLimits.ThresholdValue.Minimum} - {profileLimits.ThresholdValue.Maximum} |");
			streamWriter.WriteLine($"Available threshold types for **{profileAlgo.Algorithms[0].Name}** algorithm");
			streamWriter.WriteLine("\n");
			streamWriter.WriteLine("|Threshold Id | Threshold Name |");
			streamWriter.WriteLine("| - | - |");
			foreach (var thresholdType in profileLimits.ThresholdTypes)
            {
				streamWriter.WriteLine( $"| {thresholdType.TypeId} | {thresholdType.Name} |");
			}
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetLaserPowerLimits'></a>");
			var laserPower = ox.GetLaserPowerLimits();
			streamWriter.WriteLine("| Max Power | Min Power | Predefined Factors |");
			streamWriter.WriteLine("| - | - | - |");
			streamWriter.Write($"| {laserPower.MaxFactor} | {laserPower.MinFactor} | " );
			foreach (var predefPower in laserPower.PredefinedFactors)
			{
				streamWriter.Write($"{predefPower}, ");
			}
			streamWriter.WriteLine(" |");
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetExposureTimeLimits'></a>");
			var exposureTime = ox.GetExposureTimeLimits();
			streamWriter.WriteLine("| Max Exposure Time | Min Exposure Time |");
			streamWriter.WriteLine("| - | - |");
			streamWriter.WriteLine($"| {exposureTime.Maximum} | {exposureTime.Minimum} |");
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetProfileFilterLimits'></a>");
			var profileFilter = ox.GetProfileFilterLimits();
			streamWriter.WriteLine("| Max Filter Length | Min Filter Length |");
			streamWriter.WriteLine("| - | - |");
			streamWriter.WriteLine($"| {profileFilter.MaximumLength} | {profileFilter.MinimumLength} |");
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetAxesInfo'></a>");
			var axesInfo = ox.GetAxesInfo();
			streamWriter.WriteLine("| Z Axis Id | Z Axis Name| ");
			streamWriter.WriteLine("| - | - |");
			foreach (var axis in axesInfo.ZAxisDefinitions)
			{
				streamWriter.WriteLine($"| {axis.Id} | {axis.Name}| ");
			}
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetResolutionInfo'></a>");
			var resolutionInfo = ox.GetResolutionInfo();
			streamWriter.WriteLine("| X Resolution | Z Resolution |");
			streamWriter.WriteLine("| - | - |");
			streamWriter.Write("| ");
			foreach (var res in resolutionInfo.XResolutions)
			{
				streamWriter.Write($"{res}, ");
			}
			streamWriter.Write(" | ");
			foreach (var res in resolutionInfo.ZResolutions)
			{
				streamWriter.Write($"{res}, ");
			}
			streamWriter.WriteLine("|");
			streamWriter.WriteLine("\n");



			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetFieldOfViewLimits'></a>");
			var fovlimits = ox.GetFieldOfViewLimits();
			var fovinfo = ox.GetFieldOfViewInfo();
			streamWriter.WriteLine("|  |  |");
			streamWriter.WriteLine("| - | --- |");
			streamWriter.WriteLine($"| **FOV Max Height(Z)** | {fovlimits.MaxHeight} |");
			streamWriter.WriteLine($"| **FOV Min Height(Z)** | {fovlimits.MinHeight} |");
			streamWriter.WriteLine($"| **FOV Z Precision** | {fovinfo.ZPrecision} |");
			streamWriter.WriteLine($"| **FOV Z Unit** | {fovinfo.ZUnit} |");
			streamWriter.WriteLine($"| **FOV Max X** | {fovlimits.MaxXPlus} |");
			streamWriter.WriteLine($"| **FOV Min X** | {fovlimits.MaxXMinus} |");
			streamWriter.WriteLine($"| **FOV Min Width(Delta X)** | {fovlimits.MinWidth} |");
			streamWriter.WriteLine($"| **FOV X Precision** | {fovinfo.XPrecision} |");
			streamWriter.WriteLine($"| **FOV X Unit** | {fovinfo.XUnit} |");
			streamWriter.WriteLine("\n");


			streamWriter.WriteLine("## <a name='M-Baumer-OXApi-Ox-GetResamplingInfo'></a>");
			var resampleinfo = ox.GetResamplingInfo();
			streamWriter.WriteLine("|  |  |");
			streamWriter.WriteLine("| - | - |");
			streamWriter.WriteLine($"| **Max Grid Value** | {resampleinfo.MaximumGridValue} |");
			streamWriter.WriteLine($"| **Min Grid Value** | {resampleinfo.MinimumGridValue} |");
			streamWriter.WriteLine($"| **Grid Precision** | {resampleinfo.GridPrecision} |");
			streamWriter.WriteLine($"| **Grid Value Unit** | {resampleinfo.GridUnit} |");
			streamWriter.WriteLine("\n");

			streamWriter.Close();

			ox.Disconnect();
		}

		private static void ExampleImage()
		{
			// create an instance of a Ox object
			var ox = Ox.Create("192.168.0.250");

			// opens the network connection
			ox.Connect();

			// login to get access to all configuration parameters
			ox.Login("admin", "");

			// get the latest sensor camera image
			var image = ox.GetImage();

			// get additional information about the image
			var imageInfo = ox.GetImageInfo();

			// save the image to the hard disk
			image.Save("image.png");

			ox.Disconnect();
		}


		private static void ExampleProfile()
		{
			// create an instance of a Ox object
			var ox = Ox.Create("192.168.0.250");

			// opens the network connection
			ox.Connect();

			// login to get access to all configuration parameters
			ox.Login("admin", "");

			// get the latest profile
			var profile = ox.GetProfile();

			// get additional profile information (e.g. x and z unit)
			var profileInfo = ox.GetProfileInfo();

			// the intensity profile is identical to the profile
			// but contains additional intensity values for each profile point
			var intensityProfile = ox.GetIntensityProfile();

			Console.WriteLine("Profile:");
			Console.WriteLine($"\tTimestamp: {intensityProfile.TimeStamp}");
			Console.WriteLine($"\tLength: {intensityProfile.Length}");
			Console.WriteLine($"\tX-Start: {intensityProfile.XStart}");

			for (int i = 0; i < intensityProfile.Length; i++)
			{
				Console.WriteLine($"{intensityProfile.X[i] + intensityProfile.XStart}\t{intensityProfile.Z[i]}\t{intensityProfile.I[i]}");
			}

			ox.Disconnect();
		}


		private static void ExampleMeasurement()
		{
			// create an instance of a Ox object
			var ox = Ox.Create("192.168.0.250");

			// opens the network connection
			ox.Connect();

			// login to get access to all configuration parameters
			ox.Login("admin", "");

			// get the latest measurement values
			var measurement = ox.GetMeasurement();

			Console.WriteLine("Measurement:");
			Console.WriteLine($"\tTimestamp: {measurement.TimeStamp}");
			Console.WriteLine($"\tRate: {measurement.MeasurementRate}");
			for (int i = 0; i < measurement.Values.Length; i++)
			{
				if (!double.IsNaN(measurement.Values[i]))
				{
					Console.WriteLine($"\tValue[{i}]: {measurement.Values[i]}");
				}
			}

			ox.Disconnect();
		}


		private static void ExampleSoftwareTrigger()
		{
			var ox = Ox.Create("192.168.0.250");

			// opens the network connection
			ox.Connect();

			// login to get access to all configuration parameters
			ox.Login("admin", "");

			try
			{
				var triggerInfo = ox.GetTriggerInfo();

				var softwareTriggerId = triggerInfo.TriggerModes.FirstOrDefault(mode => mode.Name == "software")?.Id;

				if (softwareTriggerId is null)
				{
					Console.WriteLine("SoftwareTrigger not supported!");
					// closes the network connection
					ox.Disconnect();
					return;
				}

				ox.ConfigureTrigger((uint)softwareTriggerId, 0, 0, 0);

				for (int i = 0; i < 10; i++)
				{
					ox.Trigger(1);

					var measurement = ox.GetMeasurement();

					Console.WriteLine(measurement.TimeStamp);
					Thread.Sleep(1000);
				}

				var freeRunTriggerId = triggerInfo.TriggerModes.FirstOrDefault(mode => mode.Name == "freerun")?.Id;

				if (freeRunTriggerId is null)
				{
					Console.WriteLine("FreeRunTrigger not supported!");
					// closes the network connection
					ox.Disconnect();
					return;
				}

				ox.ConfigureTrigger((uint)freeRunTriggerId, 0, 0, 0);
			}
			catch (Exception e)
			{
				Console.WriteLine(e);
			}

			// closes the network connection
			ox.Disconnect();
		}


		private static void ExampleLoadStoreConfiguration()
		{
			// create an instance of a Ox object
			var ox = Ox.Create("192.168.0.250");

			// opens the network connection
			ox.Connect();

			ox.Login("admin", "");

			Console.WriteLine($"ExposureTime at beginning: {ox.GetExposureTime()}");

			// store current configuration at parameter slot 1
			ox.StoreParameterSetup(1);

			ox.ConfigureExposureTime(1000);

			Console.WriteLine($"ExposureTime after change: {ox.GetExposureTime()}");

			// load the previously stored configuration
			ox.LoadParameterSetup(1);

			Console.WriteLine($"ExposureTime after load: {ox.GetExposureTime()}");

			// reads the configuration as json string
			var json = ox.GetParameterSetup(1);
			Console.WriteLine(json);

			ox.Disconnect();
		}


		private static void ExampleReadConfiguration()
		{
			// create an instance of a Ox object
			var ox = Ox.Create("192.168.0.250");

			// opens the network connection
			ox.Connect();

			// login to get access to all configuration parameters
			ox.Login("admin", "");

			try
			{
				/* Sensor information */

				var sensorInfo = ox.GetSensorInfo();

				/* Communication settings */

				var processInterfacesInfo = ox.GetProcessInterfacesInfo();

				var processInterfaces = ox.GetProcessInterfaces();

				var networkConfiguration = ox.GetNetworkConfiguration();

				var udpInfo = ox.GetUdpStreamingInfo();

				var activeStreams = ox.GetActiveUdpStreams();

				var numberOfTimeServers = ox.GetNumberOfTimeServers();

				var timeServerConfiguration = ox.GetTimeServerConfiguration();

				/* Data acquisition */

				var exposureTime = ox.GetExposureTime();

				var exposureTimeLimits = ox.GetExposureTimeLimits();

				var exposureTimeResolution = ox.GetExposureTimeResolution();

				/* Profile */

				var profileFilter = ox.GetProfileFilter();

				var filterEnabled = ox.IsProfileFilterEnabled();

				var profileFilterLimits = ox.GetProfileFilterLimits();

				var profileAlgorithm = ox.GetProfileAlgorithm();

				var profileAlgorithms = ox.GetProfileAlgorithms();

				var profileParameter = ox.GetProfileAlgorithmParameters(0);

				var paramInfo = ox.GetProfileAlgorithmParamsInfo();

				var profileParametersInfo = ox.GetProfileAlgorithmParamsLimits(0);

				var resamplingGrid = ox.GetResamplingGridValue();

				var resamplingEnabled = ox.IsResamplingEnabled();

				var resamplingInfo = ox.GetResamplingInfo();

				var zaxis = ox.GetZAxis();

				var axesInfo = ox.GetAxesInfo();

				/* Trigger */

				var trigger = ox.GetTrigger();

				var triggerLimits = ox.GetTriggerLimits();

				var triggerInfo = ox.GetTriggerInfo();

				/* Resolution */

				var resolution = ox.GetResolution();

				var resolutionInfo = ox.GetResolutionInfo();

				/* Field Of View */

				var fov = ox.GetFieldOfView();

				var fovInfo = ox.GetFieldOfViewInfo();

				var fovLimits = ox.GetFieldOfViewLimits();

				var fovDistance = ox.GetFieldOfViewDistance();

				/* Laser Power */

				var laserPowerLimits = ox.GetLaserPowerLimits();

				var laserPowerInfo = ox.GetLaserPowerInfo();

				var laserPower = ox.GetLaserPower();

			}
			catch (Exception e)
			{
				Console.WriteLine(e);
			}


			// closes the network connection
			ox.Disconnect();
		}


		private static void StreamProfiles()
		{
			var ox = Ox.Create("192.168.0.250");

			OxStream stream = ox.CreateStream();
			// increase receive buffer size to make sure no packets are lost
			// receive buffer size must be adapted to the system and its load
			stream.ReceiveBufferSize = 128 * 1024;

			StreamWriter streamWriter = new StreamWriter("profiles.txt");

			Console.WriteLine("Reading profiles from 192.168.0.250:1234");
			Console.WriteLine("Please enable streaming of Z and intensity profile");
			Console.WriteLine("Press ESCAPE to exit!");
			Console.WriteLine("Press '+' or '-' to increase or decrease receive buffer size");
			Console.WriteLine("Press 'e' to print errors only");

			stream.Start();

			bool loop = true;
			bool errorsOnly = false;

			while (loop)
			{
				if (stream.ProfileAvailable)
				{
					var profile = stream.ReadProfile();

					streamWriter.WriteLine($"Timestamp: {profile.Timestamp}\t;Length: {profile.Length}");
					for (int i = 0; i < profile.Length; i++)
					{
						streamWriter.WriteLine($"Point {i}:\tX:{profile.X[i]}\tZ:{profile.Z?[i]}\tI:{profile.I?[i]}");
					}
					if (!errorsOnly)
					{
						Console.WriteLine($"{profile.Timestamp}");
					}
				}
				if (stream.ErrorOccured)
				{
					streamWriter.WriteLine($"Error: {stream.ReadError().Message}");
				}

				if (Console.KeyAvailable)
				{
					ConsoleKeyInfo key = Console.ReadKey(true);
					switch (key.Key)
					{
						case ConsoleKey.Escape:
							loop = false;
							break;
						case ConsoleKey.E:
							errorsOnly = true;
							break;
						case ConsoleKey.OemPlus:
							stream.ReceiveBufferSize *= 2;
							streamWriter.WriteLine($"Changed ReceiveBufferSize to { stream.ReceiveBufferSize}");
							break;
						case ConsoleKey.OemMinus:
							if (stream.ReceiveBufferSize > ushort.MaxValue + 1)
							{
								stream.ReceiveBufferSize /= 2;
							}
							streamWriter.WriteLine($"Changed ReceiveBufferSize to { stream.ReceiveBufferSize}");
							break;
						default:
							break;
					}
				}
			}

			stream.Stop();

			streamWriter.Close();

			stream.Close();
		}



		private static void StreamMeasurements()
		{
			var ox = Ox.Create("192.168.0.250");

			OxStream stream = ox.CreateStream();

			StreamWriter streamWriter = new StreamWriter("measurements.txt");

			Console.WriteLine("Reading measurements from 192.168.0.250:1234");
			Console.WriteLine("Please enable streaming measurement values");
			Console.WriteLine("Press ESCAPE to exit!");

			stream.Start();

			bool loop = true;

			while (loop)
			{
				if (stream.MeasurementAvailable)
				{
					var measurement = stream.ReadMeasurement();
					streamWriter.Write($"{measurement.Timestamp}\t{measurement.MeasurementRate}\t{measurement.EncoderValue}\t");
					foreach (var v in measurement.Values)
					{
						streamWriter.Write($"{v}\t");
					}
					streamWriter.WriteLine("");

					Console.WriteLine($"{measurement.Timestamp}");
				}

				if (Console.KeyAvailable)
				{
					ConsoleKeyInfo key = Console.ReadKey(true);
					switch (key.Key)
					{
						case ConsoleKey.Escape:
							loop = false;
							break;
					}
				}
			}

			stream.Stop();

			streamWriter.Close();

			stream.Close();
		}
	}
}
