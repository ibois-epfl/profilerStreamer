import sys, os
import time

# path to .NET OXApi.Dll and python wrapper
sys.path.append(os.path.dirname(os.path.abspath(__file__)) + r"\\..\\OxApi")

import oxapi


def ExampleGetSensorInfo(ox):
    sensorType, vendorName, serialNumber, aggregatedVersion, softwareVersion = ox.GetSensorInfo()
    print("### Sensor information ###")
    print(vendorName)
    print(sensorType)
    print(aggregatedVersion)
    print(serialNumber)
    print(softwareVersion)


def ExampleGetProfileAlgorithms(ox):
    algos = ox.GetProfileAlgorithms()
    print (algos)


def ExampleGetTrigger(ox):
    mode, option, time, endcoder = ox.GetTrigger()
    print(mode)
    print(option)
    print(time)
    print(endcoder)

    minTime, maxTime, minSteps, maxSteps = ox.GetTriggerLimits()
    print(minTime)
    print(maxTime)
    print(minSteps)
    print(maxSteps)

    timeUnit, modes, modeoptions, options = ox.GetTriggerInfo()
    print(timeUnit)
    print(modes)
    print(modeoptions)
    print(options)

def ExampleSetTrigger(ox):
    ox.ConfigureTrigger(2, 1, 10000, 100)

def ExampleExposureTime(ox):
    min, max = ox.GetExposureTimeLimits()
    print(min)
    print(max)
    resolution = ox.GetExposureTimeResolution()
    print(resolution)
    exp = ox.GetExposureTime()
    print(exp)




##################################################################################
##################################################################################
###                                                                            ###
### Demo to show the usage of the OX API for configuration                     ###
###                                                                            ###
##################################################################################
##################################################################################

# creates a OX object
ox = oxapi.ox("192.168.0.250")

# establish a connection to the OXP
ox.Connect()

# login is required to access some sensor resources, e.g. the raw image.
ox.Login("admin", "")    # the default password is empty, but can be changed on the web interface

try:
    
    ExampleGetSensorInfo(ox)

    ExampleGetTrigger(ox)

    ExampleSetTrigger(ox)

    ExampleExposureTime(ox)

except:
    print(sys.exc_info()[0])
    print(sys.exc_info()[1])


# always close the connection
ox.Disconnect()



