# Lab 6: Open Loop Systems
In this portion of Lab 6, we explore and implement an open loop system meant to cool a voltage regulator via a fan. In short, we took data to build a model that should set a certain fan PWM given a certain input temperature.

<img src="http://www.dfe.com/photos/openvsclosedloop.jpg" alt="Open Loop vs Closed or Feedback Loop">

Above is a simple comparison of an open loop vs a closed loop system. Our process contains code that will pick a PWM that correlates with our model. The second system in the image is a closed loop system. This will be explored later, in the milestone project.

Before we get into the nitty-gritty, the processor we chose to use for all parts of the lab was the G2553. Our defense for using this processor is that it is easily replaceable should something burn the processor out. This is a concern here since we are dealing with currents and voltages that are much higher than any of the MSP430s can handle; accidentally exposing a pin to high voltage could kill the whole processor.

## Voltage Regulator
The voltage regulator is the element of the system that will constantly be producing heat. Our setup used a 22ohm 1W power resistor at the output of the voltage regulator and 15.5V input. The voltage regulator used was the LM7805CV3.


<img src="https://cdn.discordapp.com/attachments/355174850932899840/386285769813131267/schem_vreg.png" alt="Open Loop vs Closed or Feedback Loop">

Circled above is the voltage regulator portion of our overall schematic.

## Fan Control
Our fan is a 12V DC fan pulled from a computer power supply. The fan is supplied with 15.5V DC and a simple mosfet circuit was designed to allow the low voltage of our MSP430's logic (3.3V) to switch the 15.5V supplied to the fan. By doing this, we are able to send PWM signal from the MSP430 to the gate of our mosfet to effectively make our 2 wire DC fan into a PWM driven device.

<img src="https://cdn.discordapp.com/attachments/355174850932899840/386285763651698689/schem_fan.png" alt="Open Loop vs Closed or Feedback Loop">

Circled above is the fan control system of our overall schematic.

## Temperature Reading
To read temperature, we used the LM35DT sensor. This package of the LM35 was preferred for its ability to be pressed flush against the plate of the voltage regulator to achieve more accurate and consistent temperature readings. A Binder clip was used to ensure they were always pressed together with the same force, as well as acting as a passive heatsink. The input of the LM35 was 15.5V, and the output was connected to an ADC pin on the MSP430 board.

<img src="https://cdn.discordapp.com/attachments/355174850932899840/386285769834233857/schem_temp.png" alt="Open Loop vs Closed or Feedback Loop">

Circled above is the temperature sensor portion of our overall schematic.

## System Modeling
To model our system, we manually stepped up the fan's PWM by 10% from 0-100% and recorded the LM35's reading at each step. We then graphed PWM vs Temperature. We noticed the graph was logarithmic and needed to fit a line to it that could be translated to equations for the MSP430 to use as a model. To achieve this we split the line into 3 segments giving them each their own slope. We then implemented these 3 different lines as equations in our code. This would take a temperature reading and choose PWM based on our model.
(((chart/graph/equations needed)))

<img src="https://cdn.discordapp.com/attachments/355174850932899840/386277218197700610/unknown.png" alt="Open Loop vs Closed or Feedback Loop">

The above table is the data collected for temperature at steady state vs. PWM.
<img src="https://cdn.discordapp.com/attachments/355174850932899840/386278770186780682/unknown.png" alt="Open Loop vs Closed or Feedback Loop">


Above is the plot we used to generate equations for the model.
<img src="https://cdn.discordapp.com/attachments/355174850932899840/386283713018855424/unknown.png" alt="Open Loop vs Closed or Feedback Loop">


Above are the fit line equations we got from our data.
```c
// Select correct initial PWM
    if (temp > 55)                            // beginning of the range that the first equation is valid
        PWM = (temp - 103.3) / (-1.59) * 10;  // first line equation
    else if (47 < temp < 55 )                 // range that the second equation is valid
        PWM = (temp - 64.2) / (-0.287) * 10;  // second line equation
    else
        PWM = (temp - 52.85) / (-0.0975) * 10; // last line equation
    if (PWM >= 1000) //if PWM is 1000 or more, it starts to rollover because of the CCR setup
        PWM = 1000-1;//so we force PWM to never go above 999.
```
The above code is the line equations from our system model, solved for PWM.
## Open Loop Control System
Now that we have equations in our code that fit the model, we add code to allow a desired temperature to be sent through UART. The system then uses the equations to try and match our desired temperature.
```c
    temp = UCA0RXBUF;                // Set user temp using input from UART
    UCA0TXBUF = UCA0RXBUF;           // TX -> RXed character
```
The above code is the user input temperature that is compared across our three line equations.

## Inputs and Outputs of the Open Loop Control System
The only input for this open loop system is temperature in celcius, represented in Hexidecimal. This input is done through UART. The recieve pin is P1.1.

The first output is a PWM signal that ultimately dictates the RPM of our fan. The specifics of that implementation were explained above under Fan Control. The PWM pin is P1.6, and is achieved with a hardware PWM implementation.

The second output is UART transmit to a terminal like realterm that displays the current temperature in HEX celcius as well as echos back the input. The transmit pin is P1.2

## Notes
This method is crude and is extremely susceptible to being broken by outside factors (think ambient room temperature changes, fan positioning). On the same day that we created our model, we also tested our system to see how it would respond, and our temperatures were within 5 degrees of the desired value. We tested it the next day at a different bench and not precisely the same fan positioning, and our model was practically useless. It got us within 15 degrees. This is why a closed loop system is needed. The model is not completely useless, it brings the PWM within the ball park of the target and that is an important part of the closed loop system.

