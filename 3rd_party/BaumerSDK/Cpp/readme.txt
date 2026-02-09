The OX-CPP-SDK is a set of software libraries and tools to support the integration of the OX into third party applications. 
The SDK is based on the C++ Library "libOXApi.so" which can be easily integrated into own C++ programs. 

The OX C++ SDK consists of the following parts:
- A C++ shared library (libOXApi.so) which provides access to all OX configuration and measurement possibilities.
- The C++ Header files to use the library.
- A example in C++ which demonstrate the OXApi usage.



Linux x86 - LibOxApi_XX-XX-XX.tar.gz:

Folder structure:
/usr/lib/libOXApi.so              The OX C++ SDK main shared library.
/usr/include                      The OX C++ SDK header files.
/example             	          A C++ cmake example which demonstrates the usage of the OXApi.

Notes:

OXApi:
	- libOXApi.so requires Ubuntu 18.04 LTS with Boost 1.70
	- Only one configuration connection can be established at one time, so if the Webinterface is active, the OXApi will not be able to connect.

Setup Development System:
    - Get Ubuntu 18.04 LTS (AMD64 desktop image) from https://releases.ubuntu.com/18.04/
       Note: If you only get a black screen while installation, press ESC while Grub menu and then F6 and mark "nomodeset". The start install.
    - Setup Network to get access to the internet
    - Run following command in terminal:
       sudo apt update
       sudo apt install build-essential cmake
    - Install / Upgrade libboost 1.70
       sudo add-apt-repository ppa:mhier/libboost-latest
       sudo apt update
       sudo apt install libboost1.70-dev

Use SDK example:
    - The SDK files are copied to ~/OxApi/
    - Create build directory and run build
       mkdir ~/OxApi/build
       cd ~/OxApi/build
       cmake ../example
       cmake --build .
    - Run example
       ./oxapiexamples



Windows Win32 and x64 - LibOxApi_XX-XX-XX.zip:

Folder structure:
/libs                             The OX C++ SDK main shared library.
/include/OXApi                    The OX C++ SDK header files.
/example                          A C++ cmake example which demonstrates the usage of the OXApi.

Notes:

OXApi:
    - OXApi requires Visual Studio 2017 or higher
    - Only one configuration connection can be established at one time, so if the Webinterface is active, the OXApi will not be able to connect.

UDP Streaming:
    - To use UDP Streaming, the desired streams should be activated by the OXApi or the Webinterface.
    - Ensure that the streaming target IP-Address is configured correctly.
    - If more than one OX should stream to the same computer, different ports should be used.
    - Keep in mind that the firewall may block an application from opening an UDP port.

Version history:
- V1.0.0 First release.
- V1.1.0 Bugfix udp streaming, rework example
- V2.0.0 Small API changes, bugfixes
- V2.0.1 bugfixes (udp streamer)
- V2.0.2 Introduces 32 bit values for profile udp stream (OXx200-B20 and OXx200-B02)
