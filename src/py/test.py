from bindings.Debug import profilerStreamerBindings as psb
import time

def main():
    TCPIPhost = "192.168.0.251"
    tcp_communicator = psb.TCPCommunicator(TCPIPhost, psb.DeviceType.OX)
    tcp_communicator.Connect()
    opcua_communicator = psb.OPCUACommunicator("opc.tcp://192.168.0.64:4840", psb.DeviceType.IO_LINK, (6, 229916))
    opcua_communicator.Connect()
    tcp_recorder = psb.TCPRecorder(tcp_communicator, 5)
    opcua_recorder = psb.OPCUARecorder(opcua_communicator, 5)
    recording = True
    tcp_recorder.Record(recording)
    opcua_recorder.Record(recording)
    time.sleep(10)
    recording = False
    time.sleep(0.5)
    profiles_over_time = tcp_recorder.GetRecordedData()
    rangefinder_data_over_time = opcua_recorder.GetRecordedData()
    print("################################")
    print("################################")
    print(f"Recieved {len(profiles_over_time)} profiles and {len(rangefinder_data_over_time)} distance data points")
    print("################################")
    print("################################")
if __name__ == "__main__":
    main()