# PS/2 Keypad Interfacing with Cortex-M3 (LCP1343)
[Blog Post Link](https://embeddedlaboratory.blogspot.com/2016/06/ps2-keypad-interfacing-with-lpc1343.html)  

The keyboard is the most common way for humans to input information into a computer. It has been around since before computers were mainstream and everyone was still using typewriters. Because of this prevalence in society, it's important that we understand how to interface to the basic PS/2 keyboard.  
This article will describe and show you an example of how to create a system capable of interacting with a keyboard in order to understand what keys have been pressed. The example system is built on the LPC1343 Development Board, but the PS/2 Library Code is generic and can be ported to any other microcontroller.  

![PS2 Male Connector](https://4.bp.blogspot.com/-3vUr6701unQ/V24vmvsfLwI/AAAAAAAAAQI/X0s1YrA--IYwnzighc-LcPkXhnrYxHWRQCKgB/s1600/ps2_male.jpg)

![PS2 Female Connector](https://2.bp.blogspot.com/-8cQO6NTji_M/V24vmsXNUMI/AAAAAAAAAQE/SwsYZikWY4Q3MxCnt5v9G2m5IxQp3m_5gCKgB/s1600/ps2_female.jpg)

So let's start from the basics. PS/2 is a serial protocol and only uses 2 pins to communicate information. The data pin is actually bi-directional, but we will only consider the data output from the PS/2 device.  
The standard pinout for a mini-din or PS/2 port can be seen above for both the male and female connectors. Typically the male connector is on the keyboard/mouse or whatever PS/2 device you have and the female connector will be on the receiving device (laptop/desktop) which is the LPC1343 in our case.  

![Pin Connections](https://3.bp.blogspot.com/-PjxsZtefaRw/V24vmGO4Q3I/AAAAAAAAAP0/jkWso5OoAz8ylnkq6UXqwKeGYRbFlaGmwCKgB/s1600/PS2%2BConnector%2BPins.PNG)

![PS2 Data Output](https://2.bp.blogspot.com/-roudzYnJDRY/V24vm35P4GI/AAAAAAAAAQM/NAPsZmLevNIrks1zXL5f6vng8-kzicGbACKgB/s1600/ps2_waveform.gif)

So initially, we connected PS/2 Keyboard with the LPC1343 development board and then connected the Logic Analyzer to view the pluses. The following images are from the Logic Analyzer Application and show the pulses for keypress 'A' and 'B', whenever a key is pressed then a scan code is sent by the keyboard and when the key has released the break and keyboard scan code is sent, which is shown below in the images.  
One more interesting thing is that, when a key on the keyboard is pressed, suppose 'A' is pressed, the keyboard will not send the ASCII character of 'A', in spite it will send the scan code, by this scan code we will determine which key is pressed.  

### When 'A' is pressed, 0x1C is transmitted by the keyboard.
![A is Pressed](https://1.bp.blogspot.com/-7bSAnu6vlMI/V24vlYMem1I/AAAAAAAAAPs/pBtjS-j55Nk-p_AyQYrdLG90G11xnYZ0wCKgB/s1600/A.png)

### When 'A' is released, Break(0xF0) and 0x1C is transmitted by the keyboard.
![A is Released](https://1.bp.blogspot.com/-FX_c3alom7g/V24vlfBGIoI/AAAAAAAAAQU/zGAKm8bQb0EifVEUHA0jzvmL_2fpFZbPQCKgB/s1600/Break%2BA.png)

### When 'B' is pressed, 0x32 is transmitted by the keyboard.
![B is Pressed](https://1.bp.blogspot.com/-b5eENUnKzPY/V24vlSWCFNI/AAAAAAAAAQU/QtN-VrfZ7pcQNdKCzYojJMZueuMgKnJyACKgB/s1600/B.png)

### When 'B' is released, Break(0xF0) and 0x32 are transmitted by the keyboard.
![B is Released](https://3.bp.blogspot.com/-PUzhoevHxLc/V24vmEuMqDI/AAAAAAAAAQU/7Q2dYKbh0G4iVUu4F0QONgy0ESRwsLCZgCKgB/s1600/Break%2BB.png)

## Important Points
* Configure Clock Pin to Interrupt on Falling Edge.
* When Interrupt occurs, read Data line values.
* Decode bits and store Data in a variable.
* Check Parity, if parity is correct, move data in a queue else reject data.
* Extract Scan Code from Queue and compare the scan code with look-up data to determine which key is pressed.

Have a look at this video for a demonstration of the project.
[PS2 Keyboard Interfacing with LCD1343 Youtube Video](https://www.youtube.com/watch?v%3Djp_rwXT7BWM)

## Schematic Diagram
![Schematic Diagram](https://1.bp.blogspot.com/-7Ol9Ouz9AgE/V24vmJfZ-vI/AAAAAAAAAQU/V7la3yNDUtQQ7zwDV5KZwuVFf3EdzOMEQCKgB/s1600/Schematic%2BDiagram.PNG)

Code is written in IAR for ARM version 7.60 can be ported to any other microcontroller.