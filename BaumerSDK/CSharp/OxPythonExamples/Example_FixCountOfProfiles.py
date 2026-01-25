import sys
import time
import pandas as pd
import os 
import plotly.graph_objects as go
import numpy as np
# path to .NET OXApi.Dll and python wrapper
sys.path.append(r"..\API")

import oxapi

#%% Input Parameters:
outputFolder = r"C:\Test"
outputFilename = "xxxSample"


# creates a OX object
ox = oxapi.ox("192.168.0.250")

# establish a connection to the OXP
ox.Connect()

# login is required to access some sensor resources, e.g. the raw image.
ox.Login("admin", "")    # the default password is empty, but can be changed on the web interface



COL=["profile_x", "profile_z"]
df = pd.DataFrame(columns=COL)

numberOfProfiles = 600


#%% Take profiles:
#time.sleep(2)
for ind in range(numberOfProfiles):
    try:
        
    #    qualityId, config, alarm, digitalouts, encoder, timeStamp, rate, values = ox.GetMeasurement()
    #    print("### Measurement ###")
    #    print(qualityId)
    #    print(config)
    #    print(alarm)
    #    print(digitalouts)
    #    print(encoder)
    #    print(timeStamp)
    #    print(rate)
    #    print(values)
       
        qualityId, timeStamp, precision, xStart, length, x, z = ox.GetProfile()
    #    print("### Profile ###")
    #    print(qualityId)
        print(timeStamp)
    #    print(precision)
    #    print(xStart)
    #    print(length)
        x_mm=[(xStart+x[i])/precision for i in range(length)]
        z_mm=[(z[i])/precision for i in range(length)]

        df = pd.concat([df if not df.empty else None, pd.DataFrame([{'profile_x': x_mm, 
                                                                     'profile_z': z_mm}])], ignore_index=True)
        #df=df.append({'profile_x': x_mm, 
        #              'profile_z': z_mm}, ignore_index=True)
    except:
        print(sys.exc_info()[0])
        print(sys.exc_info()[1])
ox.Disconnect()

#df1=pd.DataFrame(values)


#%% Visualization
if not os.path.exists(outputFolder):
    os.makedirs(outputFolder)

z_min = np.inf
z_max = -np.inf
try:
    for ind in range(numberOfProfiles):
        if z_min > np.min(df.iloc[ind]['profile_z']):
            z_min = np.min(df.iloc[ind]['profile_z'])
        if z_max < np.max(df.iloc[ind]['profile_z']):
            z_max = np.max(df.iloc[ind]['profile_z'])
except:
    raise ValueError("It seems that at least one profile is empty. Ensure there is a valid profile for all measurements.")
fig = go.Figure()
for ind in range(numberOfProfiles):
    fig.add_trace(go.Scatter3d(x=df.iloc[ind]['profile_x'], 
                               y=[ind]*len(df.iloc[ind]['profile_x']),
                               z=df.iloc[ind]['profile_z'],
                               marker = dict(
                                   size = 5,
                                   color=df.iloc[ind]['profile_z'],
                                   cmin = z_min,
                                   cmax = z_max,
                                   colorscale="Viridis"),
                               mode = 'markers'))
fig.update_layout(scene = dict(
    xaxis_title = "X Direction", 
    yaxis_title = "Time",
    zaxis_title = "Z Direction"),)

fig.update_layout(scene=dict(
            aspectmode = 'manual', 
                  aspectratio=dict(x=2, y=5, z=1)))
fig.write_html(os.path.join(outputFolder, outputFilename + ".html"), auto_open=True)

df.to_csv(os.path.join(outputFolder, outputFilename + ".csv"))

#%%


# COL=["profile_x", "profile_z"]
# data={"profile_x":x_mm, "profile_z":z_mm}
# COL_Z[2]=["profile_z2"]
# data_Z={"profile_z2":z_mm}
# df=pd.DataFrame(data, columns=COL)
# dfZ=pd.DataFrame(data_Z, columns=COL_Z)

# #create new file named output.xlsx in current directory
# # create excel writer object
# path_to_file=os.path.join(r"C:\test", "test.xlsx")
# writer = pd.ExcelWriter(path_to_file)

# df.to_excel(writer, sheet_name="ProfileData")

# df.to_csv(r"c:/test/test.csv")
# dfZ.to_excel(writer, sheet_name="ProfileData")
# #df1.to_excel(writer, sheet_name="MeasurmentResults")

# # save the excel
# writer.save()


# # always close the connection




