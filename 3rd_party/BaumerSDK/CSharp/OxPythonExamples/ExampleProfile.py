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
    
    maxLen, xunit, zunit = ox.GetProfileInfo()
    print("### Profile info ###")
    print(maxLen)
    print(xunit)
    print(zunit)
    
    qualityId, timeStamp, precision, xStart, length, x, z = ox.GetProfile()
    print("### Profile ###")
    print(qualityId)
    print(timeStamp)
    print(precision)
    print(xStart)
    print(length)
    x_mm=[(xStart+x[i])/precision for i in range(length)]
    z_mm=[(z[i])/precision for i in range(length)]
    print(x_mm)
    print(z_mm)

except:
    print(sys.exc_info()[0])
    print(sys.exc_info()[1])


# always close the connection
ox.Disconnect()



