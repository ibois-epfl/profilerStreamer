import sys
import time
import os

import open3d as o3d

# path to .NET OXApi.Dll and python wrapper
api_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "./BaumerSDK/CSharp/API"))
sys.path.append(api_path)
import oxapi


def get_precision(ox):
    """
    gets the precision of the profiler
    
    :param ox: base object of the OXApi
    :return: precision value
    """
    ox.Connect()
    qualityId, timeStamp, precision, xStart, length, x, z = ox.GetProfile()
    ox.Disconnect()
    return precision


def read_profiles(ox,
                  precision, 
                  time_interval=0.1,
                  max_profiles=500):
    """
    reads profiles from the OX device and creates a point cloud

    :param ox: base object of the OXApi
    :param precision: precision value of the profiler
    :param time_interval: time interval between profile reads
    :param max_profiles: maximum number of profiles to read
    :return: open3d point cloud object
    """
    stream = ox.CreateStream()

    stream.SetReceiveBufferSize(10000000)
    size = stream.GetReceiveBufferSize()

    stream.Start()

    stream.ClearProfileQueue()
    time.sleep(time_interval)

    pcd = o3d.geometry.PointCloud()
    points = []
    intensity = []
    for i in range(0, max_profiles):
        time.sleep(time_interval)
        if stream.GetProfileCount() > 0:
            blockId, confiMode, ntpSync, valid, alarm, quality, timestamp, length, encoder, x, z, intensity = stream.ReadProfile()
            x = [k/precision for k in x]
            z = [k/precision for k in z]
            print(timestamp)
            for j in range(0, len(x)):
                points.append([x[j], i, z[j]])
                intensity.append(intensity[j])
        else:
            print("No profile recieved")

    c_max = max(intensity)
    pcd.points = o3d.utility.Vector3dVector(points)
    pcd.colors = o3d.utility.Vector3dVector([[c/c_max, c/c_max, c/c_max] for c in intensity])
    stream.Stop()

    return pcd


def save_point_cloud(pcd, filename="ox_pointcloud.ply"):
    """
    saves the point cloud to a ply file
    
    :param pcd: open3d point cloud object
    :param filename: filename to save the point cloud
    """
    o3d.io.write_point_cloud(filename, pcd)


def visualize_point_cloud(pcd):
    """
    visualizes the point cloud using open3d

    :param pcd: open3d point cloud object
    """
    o3d.visualization.draw_geometries([pcd])
    

def main():
    ox = oxapi.ox("192.168.0.251")
    precision = get_precision(ox)
    pcd = read_profiles(ox, precision, time_interval=0.05, max_profiles=500)
    save_point_cloud(pcd, "ox_pointcloud.ply")
    visualize_point_cloud(pcd)

if __name__ == "__main__":
    main()
