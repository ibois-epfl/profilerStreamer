# profilerStreamer
profilerStreamer is a small project to combine 2D laser profiler data from Baumer with 1D sick distance sensor to recreate 3D point cloud of timber pieces in a CNC.

The hardware used in this instance is:

- [Baumer OXP200 profiler](https://www.baumer.com/ch/fr/apercu-des-produits/smart-vision/capteurs-de-profil/ox-laser-bleu/oxp200-b20c-004/p/46978)
- [Sick DT50-2B215252 distance sensor](https://www.sick.com/ch/en/catalog/products/distance-sensors/laser-distance-sensors/dx50-2/dt50-2b215252/p/p356454?tab=detail)

> [!CAUTION]
> The Sick sensor integration will be done with a IO-LINK master and is currently not implemented.

## Usage
> [!NOTE]  
> For now this repo is only a python script working on Windows.

### UV
We use [`uv`](https://docs.astral.sh/uv/) as package manager. Please  [install `uv`](https://docs.astral.sh/uv/) before trying to run.

### Network setup
To communicate with the profiler's web socket, connect your computer to local network (the one in which the profiler is present), which likely implies disabling WIFI and connecting ethernet cable. Then modify your computer's settings as follows:

![image](./assets/ethernet_settings_to_specify.png)

Once this setup specified, you can turn the profiler ON, and the connection should work fine.

### Running the code:
To run the small script, please execute in terminal from root of this repo: 

```bash
uv run .\StreamProfiles_2.0.py
```