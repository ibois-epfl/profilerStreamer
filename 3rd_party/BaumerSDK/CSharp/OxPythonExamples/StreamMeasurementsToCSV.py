import sys
import time
import pandas as pd
import os 

# path to .NET OXApi.Dll and python wrapper
sys.path.append(r"..\API")

import oxapi


# Input Parameters:
outputFolder = r"C:\Test"
outputFilename = "MeasurementList"
numberOfProfileMeasurments = 200

COL=["Timestamp", "Measurement 1", "Measurement 2", "Measurement 3", "Measurement 4", "Measurement 5", "Measurement 6", "Measurement 7", "Out 1", "Out 2"]
df = pd.DataFrame(columns=COL)

# creates a Ox object
ox = oxapi.ox("192.168.0.250")

stream = ox.CreateStream()

stream.Start()

stream.ClearMeasurementQueue()

for i in range(numberOfProfileMeasurments):
    time.sleep(0.05)
    if stream.GetMeasurementCount() > 0:
        blockId, configMode, timestamp, sync, valid, quality, alarm, outs, rate, encoder, values = stream.ReadMeasurement()
        #print(timestamp)
        #print(valid)
        #print(alarm)
        #print (values)
        df = pd.concat([df if not df.empty else None, pd.DataFrame([{'Timestamp': timestamp,
                                                                     'Measurement 1': values[0],
                                                                     'Measurement 2': values[1],
                                                                     'Measurement 3': values[2],
                                                                     'Measurement 4': values[3],
                                                                     'Measurement 5': values[4],
                                                                     'Measurement 6': values[5],
                                                                     'Measurement 7': values[6],
                                                                     'Out 1': outs[0],
                                                                     'Out 2': outs[1]}])], ignore_index=True)
stream.Stop()

if not os.path.exists(outputFolder):
    os.makedirs(outputFolder)
    
df.to_csv(os.path.join(outputFolder, outputFilename + ".csv"))
print(os.path.join(outputFolder, outputFilename + ".csv"))



