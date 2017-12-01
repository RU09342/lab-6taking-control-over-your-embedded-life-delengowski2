# Lab 6: "High Power" Control
The goal of this lab is to drive devices that require voltages/currents that are too much for a MSP430 to sink/source. Essentially it is voltages/currents that will otherwise damage the MSP430 if they were to be used. For example, under the mixed signal microcontroller datasheet for the G2, the absolute maximum voltage that can be applied to any pin is -0.3 volts to Vcc + 0.3 volts. Additionally the maximum current that can be drawn is 420 uA.

The solution to this is to utilize electronically controlled switches. These switches can be either relays or MOSFETs. The relay and MOSFET (assuming the later is biased correctly), require voltages that the G2 can operate safely with, but have the luxury of being able to pass or impede much higher voltages/currents than the G2 would be able to. 

# MOSFET SWITCH

The decided load was a 750-ohm power resistor. To control the current through the power resistor, the resistor was connected to the source of a 2N7000 N channel MOSFET, and pin 1.0 (LED) of the GPIO on the MSP430G2553 was connected to the gate, the drain had 12v supplied to it. The code used was a simple blink where the the blink was controlled by a timer on the G2. The current through the power resistor was measured to be 5mA. Below is an image of the circuit. 
![alt text](https://i.imgur.com/pyymNN6.png?1)

# RELAY SWITCH
Again the decided load was a 750-ohm power resistor. To control the current through the resistor a electro-mechanical relay was used however, the relay requires 12 volts to run. Therefore to control the relay with the G2 required to control a NMOS with the G2 and have the NMOS control the relay. The same code for the MOSFET switch was used for the relay switch. Below is an image of the circuit, and ossciloscope readings. The highest frequency that was usable for relay was 725 hz. 

![alt text](https://i.imgur.com/5F8q3LU.png?1)
![alt text](https://i.imgur.com/MfQhnaV.png)
![alt text](https://i.imgur.com/i5KQe6b.png)
