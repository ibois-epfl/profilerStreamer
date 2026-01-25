The OX-SDK is a set of software libraries and tools to support the integration of the OX into third party applications. 
The SDK is based on the .NET Library 'OXApi.dll' which can be easily integrated into own C# or VB programs or standard software tools like Matlab or TestStand. 
In addition there is a python wrapper (oxapi.py) available for the 'OXApi.dll' to use all their features from a python environment.

The OX SDK consists of the following parts:
- A .NET assembly (OXApi.dll) which provides access to all OX configuration and measurement possibilities.
- A python wrapper which allows the use of OxAPI.dll in a python environment. 
- A set of examples in C# and Python which demonstrate the OXApi usage.

Folder structure:
\API\OXApi.dll                    The OX SDK main assembly.
\API\oxapi.py                     Python wrapper for OXApi.dll.
\API\Newtonsoft.Json.dll          Third party Json parser (MIT license, see newtonsoft.json.txt).
\API\websocket-sharp.dll          Third party Websocket library (MIT license, see websocket-sharp.txt).

\OxApiExamples             	A C# project which demonstrates the usage of the OXApi.
\OxPythonExamples          	Python examples which demonstrate the usage of the OXApi python wrapper.

Notes:

OXApi:
	- The OXApi.dll requires Microsoft .NET Framework 4.6.1		https://www.microsoft.com/de-ch/download/details.aspx?id=49982
	- Only one configuration connection can be established at one time, so if the Webinterface is active, the OXApi will not be able to connect.

Python:
	- You have to add the path of the OXApi.dll and oxapi.py to your python path (e.g. sys.path.append(r"C:\Program Files\Baumer\OXSDK\API").
	- In order to use the wrapper, pythonnet (>= 2.4) has to be installed in your python environment. (https://pypi.org/project/pythonnet/)

UDP Streaming:
	- To use UDP Streaming, the desired streams should be activated by the OXApi or the Webinterface.
	- Ensure that the streaming target IP-Address is configured correctly.
	- If more than one OX should stream to the same computer, different ports should be used.
	- Keep in mind that the windows firewall may block an application from opening an UDP port.


Version history:
- V1.0.0 First release.
- V1.0.2 Changed measurement data layout, this is the minimum SDK version for OX firmware version V1-0-7 or greater.
- V1.0.3 Fixed problem with OXAPI.WriteAllSettings()
- V2.0.0 Small API changes, bugfixes
- V2.0.2 Fixed data conversion measurement values in udp stream
- V2.0.3 Introduces 32 bit values for profile udp stream (OXx200-B20 and OXx200-B02)