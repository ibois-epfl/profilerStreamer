"""This component calibrates the transformation from device coordinates to machine coordinates."""
#! python3

import System
import typing
import time
import gc

import Rhino
import Grasshopper
from Grasshopper.Kernel import GH_RuntimeMessageLevel as RML

import ghpythonlib.treehelpers as th
from ghpythonlib.componentbase import executingcomponent as component

from profiler_streamer import binding as psb
import diffCheck
from diffCheck import df_cvt_bindings as df_cvt
import numpy as np

class ProfilerStreamerCalibrate(component):
    def create_output_component(self):
        n_outs = len(ghenv.Component.Params.Output)
        data_component = Grasshopper.Kernel.Special.GH_Relay()
        p_manager = Grasshopper.Kernel.GH_Component.GH_OutputParamManager
        Grasshopper.Instances.ActiveCanvas.Document.AddObject(data_component, False)
        ghenv.Component.Params.Output[0].AddSource(data_component)  # noqa: F821
        # result = p_manager.AddGenericParameter( "o_calibration_data", "o_calibration_data data", "calibreation data of ProfilerStreamer", Grasshopper.Kernel.GH_ParamAccess.item)
        

    def RunScript(self,
            i_activate_component: bool,
            i_profiler_ip_address: str,
            i_opcua_address: str,
            i_recording_duration: float,
            i_trajectory: System.Collections.Generic.List[Rhino.Geometry.Line],
            i_timeout: float,
            i_slicing_interval: float,
            i_reference_points: System.Collections.Generic.List[Rhino.Geometry.Point3d],
            i_reference_targets: System.Collections.Generic.List[Rhino.Geometry.Brep]) -> typing.List[System.Object]:
        # self.create_output_component()
        if not i_activate_component:
            
            return [None, None]
        
        if i_slicing_interval is None:
            i_slicing_interval = 2.0
        if i_timeout is None:
            i_timeout = 15.0

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
        rh_pc = Rhino.Geometry.PointCloud()

        pc_as_list = sorted(pc_as_list, key=lambda segment: len(segment), reverse=True)
        
        pc_as_list_of_point3d = []
        ghenv.Component.AddRuntimeMessage(RML.Warning, "The scan is scaled 3.35 in the y and z directions. This is because it seems the sensor underestimates the measurements by that much... bummer baumer.")  # noqa: F821
        for pt in pc_as_list[0]:
            pc_as_list_of_point3d.append(Rhino.Geometry.Point3d(pt[0], pt[1] * 1.033, pt[2] * 1.033))
        rh_pc.AddRange(pc_as_list_of_point3d)

        guid = Rhino.RhinoDoc.ActiveDoc.Objects.AddPointCloud(rh_pc)

        selected_points = []
        go = Rhino.Input.Custom.GetObject()
        go.SetCommandPrompt("Select the points in the point cloud that correspond to the points inputted in the component.")
        go.GetMultiple(1, 0)
        for i in range(go.ObjectCount):
            selected_points.append(go.Object(i).Point().Location)
        Rhino.RhinoDoc.ActiveDoc.Objects.Delete(guid, True)
        source = [np.asarray([pt.X, pt.Y, pt.Z]) for pt in selected_points]
        target = [np.asarray([pt.X, pt.Y, pt.Z]) for pt in i_reference_points]
        first_transform = psb.Utils.ComputeTransformationMatrix(source, target)
        first_rh_transform = Rhino.Geometry.Transform(1)
        for i in range(4):
                for j in range(4):
                    first_rh_transform[i, j] = first_transform[i, j]
        rh_pc.Transform(first_rh_transform)
        if i_reference_targets:
            bboxes = [target.GetBoundingBox(False) for target in i_reference_targets]
            source_df_cloud = diffCheck.diffcheck_bindings.dfb_geometry.DFPointCloud()
            target_df_cloud = diffCheck.diffcheck_bindings.dfb_geometry.DFPointCloud()
            for i, bb in enumerate(bboxes):
                transform = Rhino.Geometry.Transform.Scale(bb.Center, 2)
                bb.Transform(transform)
                vertices = bb.GetCorners()
                bb_as_array = [np.asarray([vertice.X, vertice.Y, vertice.Z]) for vertice in vertices]
                df_cloud = df_cvt.cvt_rhcloud_2_dfcloud(rh_pc)
                df_cloud_copy = df_cloud.duplicate()
                df_cloud.crop(bb_as_array)
                source_df_cloud.add_points(df_cloud)
                meshing_parameters = Rhino.Geometry.MeshingParameters.DefaultAnalysisMesh
                meshes = Rhino.Geometry.Mesh.CreateFromBrep(i_reference_targets[i], meshing_parameters)
        
                unified_mesh = Rhino.Geometry.Mesh()
                for mesh in meshes:
                    unified_mesh.Append(mesh)
                unified_mesh.Faces.ConvertQuadsToTriangles()
                df_mesh = df_cvt.cvt_rhmesh_2_dfmesh(unified_mesh)
                df_cloud = df_mesh.sample_points_uniformly(2000)
                target_df_cloud.add_points(df_cloud)

            source_df_cloud.estimate_normals(search_radius=5)
            target_df_cloud.estimate_normals(search_radius=5)
            RELATIVE_FITNESS = 1e-6
            RELATIVE_RMSE = 1e-6
            df_xform = diffCheck.diffcheck_bindings.dfb_registrations.DFRefinedRegistration.O3DGeneralizedICP(
                source=source_df_cloud,
                target=target_df_cloud,
                max_correspondence_distance=20,
                max_iteration=100,
                relative_fitness=RELATIVE_FITNESS,
                relative_rmse=RELATIVE_RMSE
            )
            df_xform_matrix = df_xform.transformation_matrix
            second_rh_transform = Rhino.Geometry.Transform()
            for i in range(4):
                for j in range(4):
                    second_rh_transform[i, j] = df_xform_matrix[i, j]
            if second_rh_transform == Rhino.Geometry.Transform.Identity:
                ghenv.Component.AddRuntimeMessage(RML.Warning, "The transformation matrix is identity, no transformation is applied")  # noqa: F821
                
            total_rh_transform = second_rh_transform * first_rh_transform

        x = i_trajectory[0].From.X
        y = i_trajectory[0].From.Y
        return [[total_rh_transform, x, y], rh_pc]