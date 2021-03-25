# Installation


* Clone this git repository :
```
  git clone https://github.com/MOLOCH-dev/Wall-E-Sim.git
```
* Download MinGW on Windows following [these instructions](https://code.visualstudio.com/docs/cpp/config-mingw) 

# Run the code

Run the following commands on cmd
(path = path to cloned repo, for eg. C:\Users\name\Desktop\Wall_E_CoppeliaSim)
* Open cmd
```
  cd path\Task_2
  cmake -G "MinGW Makefiles" -Bpath\Task_2\bin
  cd bin
  mingw32-make
```

* Open the scene in CoppeliaSim
* In the same cmd, run
```
  Wall_E_CoppeliaSim.exe
```

