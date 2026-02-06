import sys, os
import time

# path to .NET OXApi.Dll and python wrapper
sys.path.append(os.path.dirname(os.path.abspath(__file__)) + r"\\..\\OxApi")

import oxapi

# creates a OX object
ox = oxapi.ox("192.168.0.250")

# establish a connection to the OXP
ox.Connect()

# login is required to access some sensor resources, e.g. the raw image.
ox.Login("admin", "")    # the default password is empty, but can be changed on the web interface

try:

    print("ExposureTime at beginning:\t" + str(ox.GetExposureTime()))

    # store current configuration at parameter slot 1
    ox.StoreParameterSetup(1)

    ox.ConfigureExposureTime(1000)

    print("ExposureTime after change:\t" + str(ox.GetExposureTime()))
    
    # load the previously stored configuration
    ox.LoadParameterSetup(1)

    print("ExposureTime after load:\t" + str(ox.GetExposureTime()))

    # reads the configuration as json string
    json = ox.GetParameterSetup(1)
    print(json)


except:
    print(sys.exc_info()[0])
    print(sys.exc_info()[1])


# always close the connection
ox.Disconnect()



