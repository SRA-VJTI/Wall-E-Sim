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
  cmake -G "MinGW Makefiles" -B bin
  cd bin
  mingw32-make
```

* Open the scene in CoppeliaSim
* In the same cmd, run
```
  Wall_E_CoppeliaSim.exe
```
# Proportional-Integral-Derivative (PID) 

PID is the mechanism through which we can control the motion of line following bot which is dependent on few terms given below in the equation

![equation](https://i0.wp.com/media.visaya.solutions/2017/10/300px-PIDController_Equation.png?resize=344%2C141)

where 
  1)  **K<sub>p</sub>e(t)** is the P term which basically says that (in terms of the line) your error increases as you go away from the line.
  2) **K<sub>i</sub>∫e(t)dt** is the I term which just accumulates the error overtime by summing it continuously.
  3) **K<sub>d</sub>** is the D term and is just to damp(decrease the error) and bring back to the line , so while damping it will oscillate

  Here in the equation, **K<sub>p</sub>** , **K<sub>i</sub>**, **K<sub>d</sub>** are constants

## In Simulation ---->

So when you run the simulation you will get a PID Tuning Window where you change the values of **K<sub>p</sub>** , **K<sub>i</sub>**, **K<sub>d</sub>** and try seeing the motion of the bot


![PID Tuning](/Task_2/Files/Slider.png)

* When you put only **K<sub>p</sub>** term :

![P](/Task_2/Files/P_Value.gif)

* When you put only **K<sub>i</sub>** term :

![I](/Task_2/Files/I_Value.gif)

* When you put only **K<sub>d</sub>** term :

![D](/Task_2/Files/D_Value.gif)