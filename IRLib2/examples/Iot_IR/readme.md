# IoT_IR Internet of Things Infrared Remote

This folder contains a sample implementation of an Internet of things IR remote using Adafruit Feather M0 Wi-Fi and Arduino MKR 1000.

Recommended that you familiarize yourself with the board before proceeding with this example. For the Adafruit Feather M0 Wi-Fi the tutorial at
https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500?view=all

For the Arduino MKR 1000 see the guide at
https://www.arduino.cc/en/Guide/MKR1000

Go through all of the Wi-Fi examples in that tutorial to make sure that your board is working. You will have to edit the "arduino_secrets.h" file to include your Wi-Fi name and password.

There are two sample pages included in this folder. The file "adafruit_remote.html" emulates the Adafruit Mini Remote https://www.adafruit.com/product/389

The other sample page is "cable_and_tv.html" which has codes for a BrightHouse/Spectrum cable box using the Panasonic_Old protocol and a Samsung TV using NECx protocol.

You can simply open these files in your computer without actually hosting on a Web server. It has been tested with Internet Explorer, Google Chrome, Mozilla Firefox, and Opera Browser. Explorer will require you to click on a button to enable active X controls but the others do not. Note also Explorer will automatically resize and dock the webpage but the others do not implement that feature. 

You will have to edit the source of the html pages to configure your IP address for your device.

You can edit the codes for any remote supported by IRLib2. Each button is defined by 4 values in the table beginning at line 26. For example the first button in the Adafruit remote example reads:

			[1,0xfd00ff,0, "Vol-",189],

The first value is the protocol number. The next value is the actual code to be transmitted in hex. The next value is the number of bits which is ignored in the case of NEC protocol 1 but may be important depending on your protocol. The fourth value is the string of text to be displayed on the remote. The file value is a keystroke code which can be used to activate the button in addition to clicking on it with the mouse. In this case code 189 corresponds to the "minus sign" keyboard code. The webpage will display keyboard codes as you press them so if you want to implement a new code just press the key and the webpage will tell you is code.

Note that the display text can include HTML such as span, linebreaks using br and other formatting as you can see in the examples.

You also have to edit the URL in the webpage outline 21 so that it matches your device.

If you press the escape key, the webpage will toggle between the symbols that you defined and an indication of which keyboard characters you have assigned to that particular button.

We have implemented both a mouseclick and a keyboard press capability into this webpage so that it is easier to be used with voice control software such as Dragon Naturally Speaking. For example when I control my cable box I can speak the words "press G" to call up the on-screen guide and then I can say "Move Right 5" to move the cursor along the guide. This makes this implementation and especially useful piece of assistive technology for the disabled.

We will be writing an extensive tutorial in the Adafruit learning system about this particular project very soon.

