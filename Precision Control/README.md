# R2R DAC
![ALT TEXT](https://i.imgur.com/TQZgnZl.png)
After configuration of the R2R DAC the first 8 pins of port 3 on the MSP430FR5529 were connected respectively. To generate the triangle required the simulation of a bidirectional binary counter. This was done using polling and two nested while loops. Since the DAC is 8 bits, the incrementor variable counted up to 2^8 - 1 = 255 to create the positive slope of the triangle wave and then down to zero to create the negative slope of the triangle wave. 

```
    while(1){
        while(i<255){
            P3OUT++;
            i++;
            __delay_cycles(1000);
        }
        while(i>0){
            P3OUT--;
            i--;
            __delay_cycles(1000);
        }
    }
```
The triangle wave output along with its Fast Fourier Transform 
![ALT TEXT](https://i.imgur.com/SIra8kh.png)
A gif of this all in real time

![ALT TEXT](https://i.imgur.com/214o6Yq.gif)

# Bill of Material
9 - 2k ohm resistors (found in tech office)
7 - 1k ohm resistors (found in tech office)

## Loading Effect/PWM2
Done and implemented in other sections of the lab. 
