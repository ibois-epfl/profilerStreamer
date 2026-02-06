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
    
    timeUnit, modes, modeoptions, options = ox.GetTriggerInfo()
    print(modes)

    ox.ConfigureTrigger(3, 0, 0, 0)

    for i in range(0,10):
        ox.Trigger(1)
        qualityId, config, alarm, digitalout, encoder, timeStamp, rate, values = ox.GetMeasurement()
        print(timeStamp)
        time.sleep(1)

except:
    print(sys.exc_info()[0])
    print(sys.exc_info()[1])


# always close the connection
ox.Disconnect()



