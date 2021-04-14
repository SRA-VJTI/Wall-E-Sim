#ifdef WIN32
#include <Windows.h> //For sleep() functionality, this needs to be 1st include
#endif
#ifdef __APPLE__ 
#include <unistd.h>
#endif
#ifdef __linux
#include <unistd.h>
#endif 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define BLACK_MARGIN 0.36
#define TURN_MARGIN 0.2
#define WHITE_MARGIN 0.1
#define LEFT -1
#define ALL_BLACK 1
#define REVERSE 0
#define STOP 99
#define RIGHT 2 
#define PI 3.14
#define SLEEP_TIME

extern "C" {
#include "extApi.h"
}

using namespace std;

void mySleep(int sleepMs){
#ifdef __linux
    usleep(sleepMs * 1000);
#endif
#ifdef WIN32
    Sleep(sleepMs);
#endif
}



int isOnLine(float sensorValue){
    /* Helper Function for checking if a vision sensor detects a black line
    * Parameters :
    *   1. float sensorValue : Output of vision sensor
    * Returns :
    *    int return : 1 if sensor detects a black line, 0 if sensor does not detect a black line
    */
    return(sensorValue > TURN_MARGIN ? 1:0 );
}

int isOffLine(float sensorValue){
    /* Helper Function for checking if a vision sensor does not detect a black line
    * Parameters :
    *   1. float sensorValue : Output of vision sensor
    * Returns :
    *    int return : 1 if sensor does not detect a black line, 0 if sensor detects a black line
    */
    return(sensorValue < 0.19 ? 1:0 );
}



void turnRight(int clientID, int leftmotorHandle, int rightmotorHandle){
    /* Helper Function for bot to turn right
    * Parameters :
    *   1. int clientID : Connection ID with CoppeliaSim Server
    *   2. int leftmotorHandle : Handle of the left motor
    *   3. int rightmotorHandle : Handle of the right motor
    */
    
    simxSetJointTargetVelocity(clientID, leftmotorHandle, 0.0, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(clientID, rightmotorHandle, -60 * (PI / 180), simx_opmode_oneshot_wait);
   

}



void turnLeft(int clientID, int leftmotorHandle, int rightmotorHandle){
    /* Helper Function for bot to turn left
    * Parameters :
    *   1. int clientID : Connection ID with CoppeliaSim Server
    *   2. int leftmotorHandle : Handle of the left motor
    *   3. int rightmotorHandle : Handle of the right motor
    */
    
    simxSetJointTargetVelocity(clientID, leftmotorHandle, -60 * (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(clientID, rightmotorHandle, 0, simx_opmode_oneshot_wait);
   

}



void turnReverse(int clientID, int leftmotorHandle, int rightmotorHandle){
    /* Helper Function for bot to turn left to reverse
    * Parameters :
    *   1. int clientID : Connection ID with CoppeliaSim Server
    *   2. int leftmotorHandle : Handle of the left motor
    *   3. int rightmotorHandle : Handle of the right motor
    */
    
    simxSetJointTargetVelocity(clientID, leftmotorHandle, -60 * (PI / 180), simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(clientID, rightmotorHandle, 60 * (PI / 180), simx_opmode_oneshot_wait);
   

}
void Stop(int clientID, int leftmotorHandle, int rightmotorHandle){
    /* Helper Function for bot to stop
    * Parameters :
    *   1. int clientID : Connection ID with CoppeliaSim Server
    *   2. int leftmotorHandle : Handle of the left motor
    *   3. int rightmotorHandle : Handle of the right motor
    */
    
    simxSetJointTargetVelocity(clientID, leftmotorHandle,0, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(clientID, rightmotorHandle, 0, simx_opmode_oneshot_wait);
   

}

int readSensor(int clientID, simxInt sensorHandle[], int n, simxFloat sensorValue[], int all_black_flag) {
    /* Function to read values of the three vision sensors
    * Parameters :
    *   1. int clientID : Connection ID with CoppeliaSim Server
    *   2. simxInt sensorHandle : Handles of the vision sensors
    *   3. int n : number of vision sensors used (3)
    *   4. simxFloat sensorValue : Float buffer to store sensor readings
    */
    float* auxvalue = NULL;
    int* auxvaluecount = 0;
    // Resetting sensor readings
    sensorValue[0] = 0;
    sensorValue[1] = 0;
    sensorValue[2] = 0;
    for (int i = 0; i < n; i++) {
        if (simxReadVisionSensor(clientID, sensorHandle[i], NULL, &auxvalue, &auxvaluecount, simx_opmode_buffer) == simx_return_ok) {
            // auxvalue[10] measures the average intensity of the colours detected by the vision sensor
            sensorValue[i] = 1 - auxvalue[10];

            
        }
    }
    if (sensorValue[0] >= 0.15 and sensorValue[2] >= 0.15){
        all_black_flag = 1;
        cout << "All BLACK FLAG : " << all_black_flag << endl;
    }
    else {
        all_black_flag = 0;
    }
    return all_black_flag;
 }



int callTurn(int clientID, simxInt sensorHandle[] ,int n,int leftmotorHandle, int rightmotorHandle, int all_black_flag, int all_black_counter, simxFloat sensorValue[], int turn){

    /* Function to read values of the three vision sensors
    * Parameters :
    *   1. int clientID : Connection ID with CoppeliaSim Server
    *   2. simxInt sensorHandle : Handles of the vision sensors
    *   3. int n : number of vision sensors used (3)
    *   4. int leftmotorHandle : Handle of the left motor
    *   5. int rightmotorHandle : Handle of the right motor
    *   6. int all_black_flag : Automatically updated variable whose value is 1 if all three sensors detect a black line, 0 if not
    *   7. int all_black_counter : Automatically updated variable counting the number of ALL_BLACK_FLAGS triggered consecutively.
                                (if all_black_counter = STOP, stop the simulation)
    *   8. simxFloat sensorValue : Float buffer to store sensor readings
    *   9. int turn : condition for turn (LEFT,ALL_BLACK,RIGHT or REVERSE)
    * Returns :
    *   int all_black_counter : if all_black_counter == STOP, stop the simulation
    */
    if (turn == LEFT){
        if (sensorValue[1] > TURN_MARGIN and sensorValue[2] > TURN_MARGIN){
           cout << "Turn Left" << endl;
           turnLeft(clientID, leftmotorHandle, rightmotorHandle);
           mySleep(1000);
           do {
                turnLeft(clientID, leftmotorHandle, rightmotorHandle);

                all_black_flag = readSensor(clientID, sensorHandle, n, sensorValue, all_black_flag);

                
                }while((isOffLine(sensorValue[1])));
            }
        else {
            cout << " 'LEFT'  is not the correct condition for this case " << endl;
        }
        return 0;
    }

    if (turn == RIGHT){
        if (sensorValue[1] > TURN_MARGIN and sensorValue[0] > TURN_MARGIN){
           cout << "Turn Right" << endl;
           turnRight(clientID, leftmotorHandle, rightmotorHandle);
           mySleep(1000);
           do {
                turnRight(clientID, leftmotorHandle, rightmotorHandle);

                all_black_flag = readSensor(clientID, sensorHandle, n, sensorValue, all_black_flag);

                }while((isOffLine(sensorValue[1])));
            }
        else {
            cout << " 'RIGHT'  is not the correct condition for this case " << endl;
        }
        return 0;
    }

    if (turn == REVERSE){
        cout << "Reverse" << endl;

           turnReverse(clientID, leftmotorHandle, rightmotorHandle);
           mySleep(1000);
           do {
                turnReverse(clientID, leftmotorHandle, rightmotorHandle);

                all_black_flag = readSensor(clientID, sensorHandle, n, sensorValue, all_black_flag);

                }while((isOffLine(sensorValue[1])));
        return 0;
            
    }

    if (turn == ALL_BLACK){
        turnRight(clientID, leftmotorHandle, rightmotorHandle);
        mySleep(1000);

        do {
        turnRight(clientID, leftmotorHandle, rightmotorHandle);

        all_black_flag = readSensor(clientID, sensorHandle, n, sensorValue, all_black_flag);
        if (all_black_flag == 1){
            all_black_counter++;

            if (all_black_counter > 1){
                turnLeft(clientID,leftmotorHandle,rightmotorHandle);
                mySleep(2000);
                all_black_counter = STOP;
                cout << all_black_counter << endl;
                return 99;
            }
            
        }

        }while((isOffLine(sensorValue[1])));

        return all_black_counter;

    }

    return 0;
    
}



int main()
{
    // --------------------------------- Variables ------------------------------------------------- //
    bool VERBOSE = true, isinitial = true;
    int flag = 0, clientID = 0, leftmotorHandle = 0, rightmotorHandle = 0, n = 3, stringcounter = 0, all_black_flag=0, all_black_counter=0;
    float pos = 0, error = 0, prev_error = 0, derror = 0, ierror = 0, stopcode = 0;
    //Tuned PID values
    float lineK[3] = { 15 ,1 , 0.01};
    //
    float test[5] = {10, 10, 10, 10, 0 };
    float base_vel = -60 *( PI / 180);
    int* sensorHandle;
    sensorHandle = (int*)malloc(n * sizeof(int));
    float* sensorValue;
    sensorValue = (float*)malloc(n * sizeof(float));
    // Weights assigned to sensors
    int sensorWeight[3] = {-1, 0, 1 };
    //
    float* target_vel;
    target_vel = (float*)malloc(2 * sizeof(float));
    simxInt returncode, signalLength, pingTime;
    const simxChar* signalName = "PIDvalues";
    simxUChar* signalValue;
    string PIDval[5];
    // --------------------------------------------x--------------------------------------------------- //


    // Ending all previous connections with Coppeliasim
    simxFinish(-1);                   

    // Starting main connection to Coppeliasim server as a client on port 19997, storing connection ID in variable clientID
    clientID = simxStart((simxChar*)"127.0.0.1", 19997, true, true, 5000, 5);

    mySleep(1000);

    if (clientID != -1)
    {
        cout << " Connection status to VREP: SUCCESS" << endl;


        // Starting the Coppeliasim simulation
        int start = simxStartSimulation(clientID, simx_opmode_blocking);
        if (start == simx_return_ok){
            cout << "Simulation Started In CoppeliaSim" << endl;
        }


        /* An object handle is an integer ID associated with an object in the scene
         * These IDs are automatically assigned by Coppeliasim
         * We obtain these IDs using a 'Blocking Function Call'
         * Which instructs the client to wait for a response from Coppeliasim server
         * before continuing to execute the code
        */
        int LEFTMOTOR = simxGetObjectHandle(clientID, "DynamicLeftJoint", &leftmotorHandle, simx_opmode_oneshot_wait);
        int RIGHTMOTOR = simxGetObjectHandle(clientID, "DynamicRightJoint", &rightmotorHandle, simx_opmode_oneshot_wait);
        int LEFTSENSOR = simxGetObjectHandle(clientID, "LeftSensor", &sensorHandle[0], simx_opmode_oneshot_wait);
        int MIDDLESENSOR = simxGetObjectHandle(clientID, "MiddleSensor", &sensorHandle[1], simx_opmode_oneshot_wait);
        int RIGHTSENSOR = simxGetObjectHandle(clientID, "RightSensor", &sensorHandle[2], simx_opmode_oneshot_wait);


        if (VERBOSE)
        {
            cout << "Testing New Build" << endl;
            cout << "Computed object handle for left motor : " << LEFTMOTOR << "  " << leftmotorHandle << endl;
            cout << "Computed object handle for right motor : " << RIGHTMOTOR << "  " << rightmotorHandle << endl;
            cout << "Computed object handle for left sensor : " << LEFTSENSOR << "  " << sensorHandle[0] << endl;
            cout << "Computed object handle for middle sensor : " << MIDDLESENSOR << "  " << sensorHandle[1] << endl;
            cout << "Computed object handle for right sensor : " << RIGHTSENSOR << "  " << sensorHandle[2] << endl;

        }

        // -------------------------- Initialising Sensors and Signals ----------------------------------------- //

        /* Before reading the sensors from the server, they have to be initialised on the client side
        * by reading sensor output once in a 'Non-Blocking Function' Call
        * the client need not wait for a response from Coppeliasim before executing further code
        * Here, we are initiasing the 3 vision sensors used to detect the line
        * Signals are just data streams sent from the Coppeliasim server to the C++ client
        * Here, we are initialising a stream signal containing the values for Kp, Ki, Kd, bot velocity and 'stop'
        */

        simxFloat* auxvalue = NULL;
        simxInt* auxvaluecount = 0;
        for (int i = 0; i < n; i++) {
            simxReadVisionSensor(clientID, sensorHandle[i], NULL, &auxvalue, &auxvaluecount, simx_opmode_streaming);
        }

        returncode = simxGetStringSignal(clientID, signalName, &signalValue, &signalLength, simx_opmode_streaming);

        // ----------------------------------------------x------------------------------------------------------- //


        // ---------------------------------- Initialising Joints ---------------------------------------------- //
        /* Servo motors are actually 'Revolute Joints' in CoppeliaSim
         * Here, we are initialising the Left and Right Motor joints by setting their velocities to zero
         * This is carried out in a 'Blocking Function Call'
        */



        simxSetJointTargetVelocity(clientID, leftmotorHandle, 0.0, simx_opmode_oneshot_wait);
        simxSetJointTargetVelocity(clientID, rightmotorHandle, 0.0, simx_opmode_oneshot_wait);

        cout << "Motors initialised" << endl;

        // -----------------------------------------------x------------------------------------------------------ //



        


        // ----------------------------------- Bot Control Loop ------------------------------------------------ //
        while (1) {


            // ------------------------------------ UI buttons --------------------------------------------------- //

            /* We have initialised the PID signal
            * Now we can read and process the values of the signal 
            * This is done in a 'Blocking Function Call'
            */

            // Checking if the 'Stop' button was pressed in the UI

            if (stopcode == 1) {
                break;
            }
            

            returncode = simxGetStringSignal(clientID, signalName, &signalValue, &signalLength, simx_opmode_buffer);
            
            // Checking if the signal received was not a nil value

            if (returncode == simx_return_ok) {
               
                /* The delimiter for the PID signal is ':'
                * The code below splits the string using the above delimiter
                */
                for (int i = 0; i < (signalLength); i++) {

                    if (signalValue[i] == ':') {

                        // int stringcounter counts the number of substrings seperated by ':'
                        stringcounter++;
                    }

                    else {

                        // The current non-':' value is appended in a single variable
                        PIDval[stringcounter] += signalValue[i];
                    }

                    // Resetting the int stringcounter at end of parse

                    if (i == (signalLength-1)) {
                        stringcounter = 0;
                        
                    }                   
                }

                // Converting string values from the signal to floats and resetting the PIDval variable

                for (int i = 0; i < 5; i++) {
                    
                    test[i] = stof(PIDval[i]);
                    PIDval[i] = "";
 
                }

            }

            //Assign values obtained from UI
            base_vel = (test[3] != 0) ? (-test[3] * (PI / 180)) : (-60 * (PI / 180));
            lineK[0] = (test[0] != 0) ? (test[0]) : (15);
            lineK[1] = (test[1] != 0) ? (test[1]) : (1);
            lineK[2] = (test[2] != 0) ? (test[2]) : (0.01);
            stopcode = test[4];

            // -----------------------------------------------x------------------------------------------------------ //

            // ----------------------------------- Reading Vision Sensors ------------------------------------------- //

            // Function to store values of vision sensor in local buffer
            all_black_flag = readSensor(clientID, sensorHandle, n, sensorValue, all_black_flag);
            //cout << "Out of func : " << 0.36 - sensorValue[1] << endl;

            // Clearing data stored in buffers
            simxReleaseBuffer((simxUChar*)auxvalue);
            simxReleaseBuffer((simxUChar*)auxvaluecount);

            // ----------------------------------- Conditions for Turns ------------------------------------------- //

            /*

            --------------------------------------------------------------------------------------------

            POSSIBLE CONDITIONS :
             1. ALL_BLACK : All 3 vision sensors detect a line
             2. LEFT : The Left and Middle vision sensors detect a line
             3. RIGHT : The Right and Middle vision sensors detect a line
             4. REVERSE : None of the vision sensors detect a line
             5. 'break;' : The bot reaches the end of path (ALL_BLACK condition is triggered multiple times), do not pass this in CallTurn

            ---------------------------------------------------------------------------------------------

            VARIABLES DEFINED :
             1. TURN_MARGIN : The minimum output of the vision sensor if it detects a black line
             2. WHITE_MARGIN : The maximum output of the vision sensor if it detects a white line
             3. all_black_flag : is 1 if all 3 sensors detect black line, 0 if not. This variable is already updated for you. 
              !!! DO NOT UPDATE OR CHANGE THE 'all_black_flag'  VARIABLE !!!
             4. all_black_counter : Counts the number of ALL_BLACK conditions triggered. If Value is 99, 'break;' out of the loop to stop.
             5. sensorValue[0] : Output of Left Vision Sensor
             6. sensorValue[1] : Output of Middle Vision Sensor
             7. sensorValue[2] : Output of Right Vision sensor

            ----------------------------------------------------------------------------------------------

            SYNTAX FOR CALLING callTurn() FUNCTION:
             if (condition for turn){
                all_black_counter = callTurn(clientID,leftmotorHandle,sensorHandle,n,rightmotorHandle,all_black_flag,all_black_counter, sensorValue, condition);
             }
            In place of condition, you can pass in LEFT, RIGHT, ALL_BLACK, REVERSE
            !!! DO NOT PASS THE 'STOP' CONDITION into the function 'callTurn()' !!!

            ----------------------------------------------------------------------------------------------
            */
             

            
           
            // -----------------------------------------------x------------------------------------------------------ //

            // wait till all sensor values have been recieved (restarts the while loop if error is encountered)
            int sensorError = 0;
            for (int i = 0; i < n; i++) {
                //sensorValue[i] = 1 - sensorValue[i];
                if (sensorValue[i] <= 0 || sensorValue[i] >= 1) {
                    sensorError = 1;
                    break;
                }
            }
            if (sensorError) continue;
            //cout << "No sensor error " << endl;

            // -----------------------------------------------x------------------------------------------------------ //

            // ------------------------------------- Calculating Errors --------------------------------------------- //

            /* This part of the code determines which vision sensor 
            * detects the 'darkest' image (the more amount/area of line a sensor detects, the darker its image will be)
            * and accordingly, error is assigned
            */
            if (sensorValue[0]>sensorValue[2]) {
                flag = 0; //left
            }
            else if (sensorValue[0] < sensorValue[2]) {
                flag = 2; // right
            }
            else {
                flag = 1; //middle
            }
            //cout << "Flag is : " << flag << endl;

            /* 'sensorWeight'is an array defined in the 'Variables' section of the code
            * It assigns 'weights' to the three vision sensors, to determine which side of the line the bot is on
            * if bot is on the left, pos is negative
            * if bot is on the right, pos is positive
            */
            pos = sensorWeight[flag];

            /* 0.36 is the optimum value, for which the line is exactly in the center of a vision sensor
            * If the middle sensor's readings fluctuate from this, an error is assigned
            */ 
            error =  0.36 - sensorValue[1];
            //cout << error << endl;

            // signalerror sends the calculated error in a float signal to the server side
           

            // For the first run of this loop, the error for Kd and Ki remains zero
            if (isinitial == false) {
                derror = prev_error - error;
                ierror += error;
            }
            
            /* Setting a PID controlled velocity for the bot
            * If the bot is on the left of the line, the right wheel receives a greater velocity than the left
            * So the bot turns right, towards the line
            * Same goes for left
            */
            if (error > 0.05) {
                target_vel[0] = base_vel - pos * (((lineK[0] * error)+ (lineK[1] * derror) + (lineK[2] * ierror)) * float((PI / 180) * 10.0)); //greater angle 
                target_vel[1] = base_vel + pos * (((lineK[0] * error) + (lineK[1] * derror) + (lineK[2] * ierror)) * float((PI / 180) * 10.0)); //smaller angle
            }
            else {
                target_vel[0] = base_vel;
                target_vel[1] = base_vel;
            }

            // Setting joint velocities in a blocking function call

            simxSetJointTargetVelocity(clientID, leftmotorHandle, target_vel[0], simx_opmode_oneshot_wait);
            simxSetJointTargetVelocity(clientID, rightmotorHandle, target_vel[1], simx_opmode_oneshot_wait);

            prev_error = error;

            isinitial = false;

            
        }
    }

    else
    {
        cout << " Connection status to VREP: FAILED" << endl;
    }

    simxStopSimulation(clientID, simx_opmode_oneshot);
    simxGetPingTime(clientID, &pingTime);
    simxFinish(clientID);
    return clientID;
}



