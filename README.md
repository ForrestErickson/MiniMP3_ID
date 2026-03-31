# Summary
On an ESP32, krake Hardware (UART2) identify if DFPlayer or TD5580A Mini MP3 Player.  

## Details
Program Requirements asked of Chat GPT:
ESP32 WROOM embedded system. 
We have as one of our preipherials a Mini MP3 player. 
We have two types we my use. 
One is the DFRobot player an another uses the TD5580A IC. 
We use the DFRobot library. I would like setup and loop code. 
I would like the setup code to test and set a flag for the presence of the Mini MP3 player. 
I would like the setup code to test and set a flag for which type of Mini MP3 player. 
The test should NOT depend on the presence of absence of an SD card with MP3 files. 

#define MP3_RX 16  
#define MP3_TX 17  
Identify if a DFPlayer or TD5580A player is in the circuit.  


## Screen Shot
<img width="1024" alt="image" src="https://github.com/user-attachments/assets/059bfcfc-a8d4-4af5-8b8e-d5c35e7cac57" />  
Tested with out a module and with a TD5580A.

