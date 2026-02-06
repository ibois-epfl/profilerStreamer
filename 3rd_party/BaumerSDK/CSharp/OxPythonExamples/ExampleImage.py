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
    
    width, height, maxPixels = ox.GetImageInfo()
    print("### Image info ###")
    print(width)
    print(height)
    print(maxPixels)
    
    roiHeight, roiWidth, rowOffset, colOffset, rowBinning, colBinning, pixels, saveImageFunc = ox.GetImage()
    print("### Image ###")
    print(roiHeight)
    print(roiWidth)
    print(rowOffset)
    print(colOffset)
    print(rowBinning)
    print(colBinning)
    print(pixels)

    saveImageFunc("test.png")

except:
    print(sys.exc_info()[0])
    print(sys.exc_info()[1])


# always close the connection
ox.Disconnect()



