import sys, os
import time

# path to .NET OXApi.Dll and python wrapper
sys.path.append(os.path.dirname(os.path.abspath(__file__)) + r"\\..\\OxApi")

import oxapi

# creates a Ox object
ox = oxapi.ox("192.168.0.250")

stream = ox.CreateStream()

stream.Start()

stream.ClearMeasurementQueue()

for i in range(0, 100):
    time.sleep(0.01)
    if stream.GetMeasurementCount() > 0:
        blockId, configMode, timestamp, sync, valid, quality, alarm, outs, rate, encoder, values = stream.ReadMeasurement()
        print(timestamp)
        print (values)
stream.Stop()






