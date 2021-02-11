# Clock and PC Status(CPU & GPU Temp) monitor on Renesas YRPBRL78L12 Evaluation Board

The LCD on YRPBRL78L12 Board is used to display date-time-day, CPU and GPU temps. What is displayed on the display is controlled by the interrupt driven switches on the board.

## Board showing Time, Date, and day of the week in 12Hr Mode.

![alt text](https://github.com/adithyayuri/LCD_PC_Stat_Monitor/blob/master/images/date_time_1.jpg "LCD showing date-time")

## Board showing Time and CPU temprature.

![alt text](https://github.com/adithyayuri/LCD_PC_Stat_Monitor/blob/master/images/cpu_1.jpg "LCD showing Time and CPU temprature")

## Board showing Time and GPU temprature.

![alt text](https://github.com/adithyayuri/LCD_PC_Stat_Monitor/blob/master/images/gpu_1.jpg "LCD showing Time and GPU temprature")


## Two sections that make the application work.

### Host application that feeds structured data to evaluation board
 * Applicaion is written on Python 3.7
 * Data, time information is obtained througn datatime module in python
 * CPU and GPU temps are abtained though WMI module. OpenHardwareMonitor needs be running in the background for this to work.
 * All the above data is packed into 16byte packet protected by 7bit checksum.
 * The packed data is sent to the evaluation board though virtual serial port. The serial port python code is robust and can handle hotplug and suddedn disconnection. 
 * The python code can be compiled into an windows EXE through pyinstaller.
 * This host application currently works only on Windows.


### The MCU Evaluation board firmware 

External Low power 32.768KHz crystal, RTC, UART, LCD Controller, Interrupt Controller, System Sleep controller and GPIO are the peripherals used in the MCU. Most of the peripherals are configured through Renesas code generator.

#### Firmware (Only way to set time in the board is by running the host application)
 * RTC generates periodic interrupts every second, the LCD time information is updated in the interrupt routine.
 * UART is also interrupt driven, any byte recieved is stored in a circular buffer. 
 * If valid data(Data packet is protected by 7bit checksum) is available in Circular buffer the packet is accepted.
 * After packet is accepted the circular buffer is flushed, not necessary to do this, but done anyway.
 * On button press the display mode is changed from date/time mode to CPU temp or GPU temp monitor mode.
 * If the current display mode is CPU or GPU temp monitor mode and if the data is more than 10 seconds old, the display mode is reverted to data/time mode.
 * After this the CPU is sent to low power sleep mode.

## 3D printed case

The case of the board is 3d printed and the files are availabe in [adithyayuri/Renesas-YRPBRL78L12-case](https://github.com/adithyayuri/Renesas-YRPBRL78L12-case)

The case is printed on __Creality Ender 3 V2__

![alt text](https://github.com/adithyayuri/LCD_PC_Stat_Monitor/blob/master/images/date_time_2.jpg "LCD showing date-time")



