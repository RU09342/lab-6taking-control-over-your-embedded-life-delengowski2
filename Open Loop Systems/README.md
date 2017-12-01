# Lab 6: Open Loop Systems
In this portion of Lab 6, we explore and implement an open loop system meant to cool a voltage regulator via a fan. In short, there will be a temperature reading that we must feedback into the system to alter the PWM of a fan to adjust or maintain a temperature.

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/b/b0/General_Feedback_Loop.svg/220px-General_Feedback_Loop.svg.png" alt="Feedback Loop">

Above is a basic form of a feedback loop. Our function, shown as the P in the picture, is cooling a voltage regulator with a fan, and the feedback is a temperature reading that will alter the next output. The initial input is a desired temperature to be reached.

## Voltage Regulator
The voltage regulator is the element of the system that will constantly be producing heat. Our setup used a 22ohm 1W power resistor at the output of the voltage regulator and 15.5V input. The voltage regulator used was the LM7805CV3.
(((circuit diagram of voltage regulator needed)))

## Fan Control
Our fan is a 12V DC fan pulled from a computer power supply. The fan is supplied with 15.5V DC and a simple mosfet circuit was designed to allow the low voltage of our MSP430's logic (3.3V) to switch the 15.5V supplied to the fan. By doing this, we are able to send PWM signal from the MSP430 to the gate of our mosfet to effectively make our 2 wire DC fan into a PWM driven device.
(((mosfet circuit diagram needed)))

## Temperature Reading
To read temperature, we used the LM35DT sensor. This package of the LM35 was preferred for its ability to be pressed flush against the plate of the voltage regulator to achieve more accurate and consistent temperature readings. A Binder clip was used to ensure they were always pressed together with the same force, as well as acting as a passive heatsink. The input of the LM35 was 15.5V, and the output was connected to an ADC pin on the MSP430 board.
(((circuit for LM35 interfacing with V_regulator needed)))


## System Modeling
To model our system, we manually stepped up the fan's PWM by 10% from 0-100% and recorded the LM35's reading at each step. We then graphed PWM vs Temperature. We noticed the graph was logarithmic and needed to fit a line to it that could be translated to equations for the MSP430 to use as a model. To achieve this we split the line into 3 segments giving them each their own slope. We then implemented these 3 different lines as equations in our code. This would take a temperature reading and choose PWM based on our model.
(((code needed for the equations, chart/graph needed)))

## Open Loop Control System
Now that we have equations in our code that fit the model, we add code to allow a desired temperature to be sent through UART. The system then uses the equations to try and match our desired temperature.


## Deliverables
Your README needs to contain schematics of your system, the plot of the plot of the temperature and input voltages at the 5C steps, and a brief talk about why you chose the processor you did along with the control technique for the fan. As always, you should include a brief description of the code you generated to run the experiment. You need to also include information on how to use your control software, including what inputs it is expecting and what range of values you are expecting. At this time you are not going to need to user-proof the system, but you will for the milestone, so keep it in the back of your head.

### What your README and code doesn't need
For starters, note the fact I ask you to do this with only one board. You also do not need to give me all of your code in the README, just tell me functionally what is going on along with showing off any functions you may have made.

Your code *DOES NOT* need to perform any sort of closed loop control. Save that for the milestone. This means that your system does not need to try to actively change the fan speed without your help. You are going to essentially make your microcontroller one big Convert-o-Box to turn a desired temperature into a controllable signal, and then be able to read a temperature.
