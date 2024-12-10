# autodigimin
My Digital Theremin Project

Here is the code and wiring diagram for the digital theremin project, with autotune.  

The required parts are an ESP32 development board to generate tones and generally control things, two HC-SR04+ low-voltage ultrasonic sensors to determine positions of hands to define tone and volume, a X9C103S digital potentiometer to control volume, a Sparkfun TPA2005D1 amplifier to make things audible without trying to draw too much current from a signal pin, and an 8 ohm speaker to play the tones. (Note that to get the volume control on the amplifier to work, you have to remove a solder joint from it, as set out <A HREF=https://www.sparkfun.com/tutorials/392#volume>here</A>. I also ended up snipping off the overhang on one end of the digital potentiometer circuit board, to get it to fit within the width of the breadboard.)

The sources of libraries used to make it work are in the comments in <A href="autodigimin.ino">the code</a>.

The wiring diagram is as follows:

<img src="autodigimin_bb.png" alt="Wiring Diagram" width="500">

(Note that there are two voltage buses in this breadboard design - 5v down the left to power amplifier and potentiometer, and 3.3v down the right to power the two ultrasonic sensors, so that their outputs are of the low voltage that the ESP32 requires)

The finished article should look a bit like this (note the snipped-off left-hand end of the digital potentiometer circuit board):

<img src="IMG_1196.JPG" alt="Photo of the finished product" width="500">

And sound a bit <A href="IMG_1182%201.mov">like this</a>
