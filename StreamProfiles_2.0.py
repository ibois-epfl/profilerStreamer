import sys
import time
import os

import open3d as o3d

# path to .NET OXApi.Dll and python wrapper
api_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "./BaumerSDK/CSharp/API"))
sys.path.append(api_path)
import oxapi

precision = 100

# creates a Ox object (IP, port)
ox = oxapi.ox("192.168.0.251")

# establish a connection to websocket of OX
ox.Connect()

# Get precision of Sensor (only once neccessary)
qualityId, timeStamp, precision, xStart, length, x, z = ox.GetProfile()

ox.Disconnect()

# establish a connection to UDP (port from ox object)
stream = ox.CreateStream()

# Set Recieve Buffer size to 2 MByte for around 200 Profiles
stream.SetReceiveBufferSize(10000000)
size = stream.GetReceiveBufferSize()

stream.Start()

stream.ClearProfileQueue()
time.sleep(0.1)

pcd = o3d.geometry.PointCloud()
points = []
intensity = []
for i in range(0, 500):
    time.sleep(.1)
    if stream.GetProfileCount() > 0:
        blockId, confiMode, ntpSync, valid, alarm, quality, timestamp, length, encoder, x, z, intensity = stream.ReadProfile()
        x = [k/precision for k in x]
        z = [k/precision for k in z]
        print(blockId)
        print(timestamp)
        for j in range(0, len(x)):
            points.append([x[j], i, z[j]])
            intensity.append(intensity[j])
        i= i+1
    else:
        print("No profile recieved")
c_max = max(intensity)
pcd.points = o3d.utility.Vector3dVector(points)
pcd.colors = o3d.utility.Vector3dVector([[c/c_max, c/c_max, c/c_max] for c in intensity])
o3d.visualization.draw_geometries([pcd])
o3d.io.write_point_cloud("ox_pointcloud.ply", pcd)

stream.Stop()











