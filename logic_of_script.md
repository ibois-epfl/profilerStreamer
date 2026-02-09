# Logic
The goal is to have one point cloud after 3-4 passes.
Should this component also generate the G-code ? or have 2 components ?

### Inputs:
- number of passes. 
- y-z coordinates of translation
- offset for x coordinate
- transform from scanner coord system to CNC coord system
- IP address of baumer scanner(s)
- IP address of IO_Link master
- port on IO-Link master
- rotation axis of B-axis

### Logical steps
- Lauch the recording on the component
- Launch the G-code for scanning
- have the scanning and detect the 3-4 periods during which the scanning was done (long, steady, slow-moving periods)
- apply the appropriate rotations to the scans and stitch bach one point cloud in CNC-coordinates