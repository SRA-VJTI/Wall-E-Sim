# Wall-E-Sim
A line follower simulation created in CoppeliaSim, with a C++ interface for CoppeliaSim's Remote API

This is an simuation of [SRA's Wall-E bot](https://github.com/SRA-VJTI/Wall-E_v2.2-beta/tree/dev)

<p align="center">
  <img src="./docs/wall_E_bot.JPG" width="500"/>
</p>


# Installation

* Download CoppeliaSim Edu 4.0.0 for Windows 64-bit OS from [here](https://www.coppeliarobotics.com/files/CoppeliaSim_Edu_V4_0_0_Setup.exe) (file size - 161MB).
* Download [Visual Studio Code] (https://code.visualstudio.com/download), if you haven't already


* Clone this git repository :
```
git clone https://github.com/MOLOCH-dev/Wall-E-Sim.git
```

# Run the code
Open _scenes/WallE_line_follow.ttt_ in V-REP. You should see the Wall-E line-follower path, three vision sensor floating views, and a white floor with a black line. 

Run the executable file in _bin/Wall_E_CoppeliaSim.exe_ (Right-click -> Open). This will run the main script _src/line_follow.cpp_



Sample images of the pre-grasp:

<p align="center">
  <img src="./docs/sim_img_1.png" width="256"/>
  <img src="./docs/sim_img_2.png" width="256"/>
  <img src="./docs/sim_img_3.png" width="256"/>
  <img src="./docs/sim_img_4.png" width="256"/>
  <img src="./docs/sim_img_5.png" width="256"/>
</p>


# Todo :

