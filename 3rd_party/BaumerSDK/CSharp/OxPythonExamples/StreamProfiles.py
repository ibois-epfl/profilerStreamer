import sys
import time
import os

# path to .NET OXApi.Dll and python wrapper
api_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../API"))
sys.path.append(api_path)
import oxapi

# creates a Ox object
ox = oxapi.ox("192.168.0.251")

stream = ox.CreateStream()

stream.Start()

stream.ClearProfileQueue()

for i in range(0, 100):
    time.sleep(0.01)
    if stream.GetProfileCount() > 0:
        blockId, confiMode, ntpSync, valid, alarm, quality, timestamp, length, encoder, x, z, i = stream.ReadProfile()
        print(timestamp)

stream.Stop()






