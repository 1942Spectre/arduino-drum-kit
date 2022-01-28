# arduino-drum-kit
A simple midi drum kit made from a broken guitar hero drum kit and an Arduino Leonardo

-> This project can be done without a guitar hero drum kit and actually might be better, I did it with one because I did not have enough time to make a body and already have one. Piezo Sensors are the only important components in the guitar hero kit and they can be bought anywhere for a low price.

-> I soldered some cables to the piezo sensors inside each drum pad and connected the cables to the resistors. Resistors need to be connected to arduino and sensors in a parallel circuit as shown in the diagram.

-> It will not work on Arduino Uno without other software to disable the port it uses for serial connection to the computer and some software that converts serial data to midi signals, so I suggest using a Lenoardo or similar board that supports that type of connection.

-> Here is the tinkercad link of the circuit:

 https://www.tinkercad.com/things/iFnDFEOwQ3T 

I use FL Studio for recording or getting the sound while playing and it performs well.

I don't feel any delay but I'm not a professional musician.

I used 1M resistors.

The notes for the different drum sounds are included as comments on top of the file. You need to set the sounds to the pins.


