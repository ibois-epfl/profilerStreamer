## <a name='M-Baumer-OXApi-Ox-GetTriggerInfo'></a>
| Mode Id | Mode Name | Supported Options |
| ------- | --------- | ----------------- |
| 0 | freerun | 1 |
| 1 | extSingleShot | 4 |
| 2 | fixedTime | 1 |
| 4 | encoder | 1 |
| 3 | software |  |

| Option Id | Option Name |
| --------- | ----------- |
| 0 | ignoreSyncIn |
| 1 | runWhileSyncInLow |
| 2 | runWhileSyncInHigh |
| 3 | risingEdge |
| 4 | fallingEdge |
| 5 | bothEdges |


## <a name='M-Baumer-OXApi-Ox-GetTriggerLimits'></a>
|  Property | Value |
| - | - |
| **Max Encoder Steps** |65536 |
| **Min Encoder Steps** |3 |
| **Max Interval Time** |4000000 |
| **Min Interval Time** |500 |


## <a name='M-Baumer-OXApi-Ox-GetProcessInterfacesInfo'></a>
| Process Id | Process Name |
| - | - |
| 0 | Disabled |
| 1 | Profinet |
| 2 | EtherNetIP |


## <a name='M-Baumer-OXApi-Ox-GetUdpStreamingInfo'></a>
| UDP Stream Id | UDP Stream Name| 
| - | - |
| 0 | zProfile |
| 1 | intensityProfile |
| 2 | allMeasurementValues |


## <a name='M-Baumer-OXApi-Ox-GetProfileAlgorithms'></a>
| Algorithm Id | Algorithm Name |
| - | - |
| 0 | max |
| 1 | upper |
| 2 | lower |


## <a name='M-Baumer-OXApi-Ox-GetProfileAlgorithmParamsLimits-System-Int32-'></a>
|Algorithm Name: max | Algorithm Id: 0|
| - | - |
| **Min Peak Height Range** | 1 - 255 |
| **Min Peak Width Range** | 1 - 255 |
| **Threshold Value Range** | 0 - 100 |
Available threshold types for **max** algorithm


|Threshold Id | Threshold Name |
| - | - |
| 0 | relToMax |
| 1 | relToContrast |


## <a name='M-Baumer-OXApi-Ox-GetLaserPowerLimits'></a>
| Max Power | Min Power | Predefined Factors |
| - | - | - |
| 3 | 0,5 | 3, 2, 1, 0,5,  |


## <a name='M-Baumer-OXApi-Ox-GetExposureTimeLimits'></a>
| Max Exposure Time | Min Exposure Time |
| - | - |
| 3000 | 100 |


## <a name='M-Baumer-OXApi-Ox-GetProfileFilterLimits'></a>
| Max Filter Length | Min Filter Length |
| - | - |
| 15 | 3 |


## <a name='M-Baumer-OXApi-Ox-GetAxesInfo'></a>
| Z Axis Id | Z Axis Name| 
| - | - |
| 1 | height| 
| 0 | distance| 


## <a name='M-Baumer-OXApi-Ox-GetResolutionInfo'></a>
| X Resolution | Z Resolution |
| - | - |
| 2, 4,  | 1, 2, 4, |


## <a name='M-Baumer-OXApi-Ox-GetFieldOfViewLimits'></a>
|  |  |
| - | --- |
| **FOV Max Height(Z)** | 400 |
| **FOV Min Height(Z)** | 5 |
| **FOV Z Precision** | 10 |
| **FOV Z Unit** | mm |
| **FOV Max X** | 34 |
| **FOV Min X** | -34 |
| **FOV Min Width(Delta X)** | 5 |
| **FOV X Precision** | 10 |
| **FOV X Unit** | mm |


## <a name='M-Baumer-OXApi-Ox-GetResamplingInfo'></a>
|  |  |
| - | - |
| **Max Grid Value** | 2 |
| **Min Grid Value** | 0,2 |
| **Grid Precision** | 10 |
| **Grid Value Unit** | mm |


