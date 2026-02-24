from unittest import result

import Rhino

from profiler_streamer import psb
import threading, gc
import time

def main():
    TCPIPhost = "192.168.0.251"
    
    try:
        tcp_communicator = psb.TCPCommunicator(TCPIPhost, psb.DeviceType.OX)
        tcp_communicator.Connect()
        
    except Exception as e:
        tcp_communicator.Disconnect()
        del tcp_communicator
        gc.collect()
        print(f"Failed to connect a second time to TCP device at {TCPIPhost}: {e}")
        return

    try:
        opcua_communicator = psb.OPCUACommunicator("opc.tcp://192.168.0.64:4840", psb.DeviceType.IO_LINK, (6, 229916))
        opcua_communicator.Connect()
    except Exception as e:
        opcua_communicator.Disconnect()
        del opcua_communicator
        gc.collect()
        print(f"Failed to connect to OPC UA device: {e}")
        return
    
    tcp_recorder = psb.TCPRecorder(tcp_communicator, 5)
    opcua_recorder = psb.OPCUARecorder(opcua_communicator, 5)
    tcp_recorder.StartRecording()
    opcua_recorder.StartRecording()
    time.sleep(45)
    tcp_recorder.StopRecording()
    opcua_recorder.StopRecording()

    time.sleep(0.5)
    profiles_over_time = tcp_recorder.GetRecordedData()
    rangefinder_data_over_time = opcua_recorder.GetRecordedData()
    print(f"First profile timestamp: {profiles_over_time[0].GetTimeStampAsInt()}, first rangefinder timestamp: {rangefinder_data_over_time[0].GetTimeStampAsInt()}")
    slicer = psb.DataSlicer(profiles_over_time, rangefinder_data_over_time)
    mini, maxi = slicer.Slice(1000)
    if len(slicer.GetRangeFinderDistancesSortedIntoSegments()) == 0 or len(slicer.GetProfilesSortedIntoSegments()) == 0:
        print("No valid segments found.")
        return
    if len(slicer.GetRangeFinderDistancesSortedIntoSegments()[0]) == 0 or len(slicer.GetProfilesSortedIntoSegments()[0]) == 0:
        print("No valid segments found.")
        return
    first_sorted_rangefinder_data_timestamp = slicer.GetRangeFinderDistancesSortedIntoSegments()[0][0].GetTimeStampAsInt()
    first_sorted_profile_timestamp = slicer.GetProfilesSortedIntoSegments()[0][0].GetTimeStampAsInt()
    print(f"First sorted profile timestamp: {first_sorted_profile_timestamp}, first sorted rangefinder timestamp: {first_sorted_rangefinder_data_timestamp}")
    pc_as_list = slicer.ComputeRegularizedProfilesAsArray()
    correction_distances = slicer.GetCorrectionDistances()
    print(correction_distances)
    tcp_communicator.Disconnect()
    opcua_communicator.Disconnect()
    return pc_as_list


if __name__ == "__main__":
    a = None
    run = True
    if run:
        transform = Rhino.Geometry.Transform.RotationZYX(0, 0, 1.5 / 180 * 3.141592653589793)
        transform.M00 = -1
        transform.M11 = -1
        transform.M03 = 4834
        transform.M13 = y - 331
        transform.M23 = z - 361
        result = []
        n_seg = None
        rh_pc = Rhino.Geometry.PointCloud()
        res = main()
        for point in res:
            rh_pt = Rhino.Geometry.Point3d(point[0], point[1], point[2])
            rh_pt.Transform(transform)
            rh_pc.Add(rh_pt)
        a = rh_pc
    a = a