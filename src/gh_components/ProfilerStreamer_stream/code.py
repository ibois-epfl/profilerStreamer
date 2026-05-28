"""This component streams the profiles and rangefinder data to reconstruct a 3D point cloud."""
#! python3

import System
import typing
import time
import gc


import Rhino
import Grasshopper

import ghpythonlib.treehelpers as th
from ghpythonlib.componentbase import executingcomponent as component

from profiler_streamer import binding as psb

import numpy as np

class ProfilerStreamerStream(component):
    def RunScript(self,
            i_activate_component: bool,
            i_profiler_ip_address: str,
            i_opcua_address: str,
            i_recording_duration: float,
            i_trajectories: System.Collections.Generic.List[Rhino.Geometry.Line],
            i_slicing_interval: float,
            i_calibration_data: System.Collections.Generic.List[object]) -> typing.List[System.Object]:
        
        if not i_activate_component:
            return []
        
        if i_slicing_interval is None:
            i_slicing_interval = 2.0

        tcp_communicator = None
        try:
            tcp_communicator = psb.TCPCommunicator(i_profiler_ip_address, psb.DeviceType.OX)
            tcp_communicator.Connect()
            
        except Exception as e:
            if tcp_communicator is not None:
                tcp_communicator.Disconnect()
                del tcp_communicator
                gc.collect()
            print(f"Failed to connect a second time to TCP device at {i_profiler_ip_address}: {e}")

        opcua_communicator = None
        try:
            opcua_communicator = psb.OPCUACommunicator(i_opcua_address, psb.DeviceType.IO_LINK, (6, 229916))
            opcua_communicator.Connect()
        except Exception as e:
            if opcua_communicator is not None:
                opcua_communicator.Disconnect()
                del opcua_communicator
            gc.collect()
            print(f"Failed to connect to OPC UA device at {i_opcua_address}: {e}")
        
        tcp_recorder = psb.TCPRecorder(tcp_communicator, 5)
        opcua_recorder = psb.OPCUARecorder(opcua_communicator, 5)
        tcp_recorder.StartRecording()
        opcua_recorder.StartRecording()
        time.sleep(i_recording_duration)
        tcp_recorder.StopRecording()
        opcua_recorder.StopRecording()

        time.sleep(0.5)
        profiles_over_time = tcp_recorder.GetRecordedData()
        rangefinder_data_over_time = opcua_recorder.GetRecordedData()
        print(f"First profile timestamp: {profiles_over_time[0].GetTimeStampAsInt()}, first rangefinder timestamp: {rangefinder_data_over_time[0].GetTimeStampAsInt()}")
        slicer = psb.DataSlicer(profiles_over_time, rangefinder_data_over_time)
        mini, maxi = slicer.Slice(int(i_slicing_interval) * 1000)
        if len(slicer.GetRangeFinderDistancesSortedIntoSegments()) == 0 or len(slicer.GetProfilesSortedIntoSegments()) == 0:
            print("No valid segments found.")
            return
        if len(slicer.GetRangeFinderDistancesSortedIntoSegments()[0]) == 0 or len(slicer.GetProfilesSortedIntoSegments()[0]) == 0:
            print("No valid segments found.")
            return
        first_sorted_rangefinder_data_timestamp = slicer.GetRangeFinderDistancesSortedIntoSegments()[0][0].GetTimeStampAsInt()
        first_sorted_profile_timestamp = slicer.GetProfilesSortedIntoSegments()[0][0].GetTimeStampAsInt()
        pc_as_list = slicer.ComputeRegularizedProfilesAsArray()
        correction_distances = slicer.GetCorrectionDistances()
        tcp_communicator.Disconnect()
        opcua_communicator.Disconnect()

        n_seg = None
        results = []
        
        for i, segment in enumerate(pc_as_list):
            if len(i_trajectories) > 1:
                y_calib = i_trajectories[i].From.Y
                z_calib = i_trajectories[i].From.Z
            else:
                y_calib = i_trajectories[0].From.Y
                z_calib = i_trajectories[0].From.Z
            rh_pc = Rhino.Geometry.PointCloud()
            rh_pts = []
            for point in segment:
                rh_pt = Rhino.Geometry.Point3d(point[0], point[1], point[2])
                rh_pts.append(rh_pt)
            rh_pc.AddRange(rh_pts)
            rh_transform = i_calibration_data[0]
            rh_transform.M13 += y_calib - i_calibration_data[1]
            rh_transform.M23 += z_calib - i_calibration_data[2]
            rh_pc.Transform(rh_transform)
            results.append(rh_pc)
        return [results]