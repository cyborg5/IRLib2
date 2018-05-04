# IRLib2 A Library for Receiving, Decoding and Sending Infrared Signals Using Arduino.

IRLib2 is copyright 2017 by Chris Young. It it is a major rewrite of the original IRLib 1.x by Chris Young which in turn was based on IRremote by Ken Shirriff. See COPYRIGHT.txt for details.

This library is covered under the GNU GENERAL PUBLIC LICENSE Version 3. See LICENSE.txt for a copy of the license.

See CHANGELOG.txt for recent changes.

A detailed Users Manual consisting of a reference, several tutorials, and information on how to implement new protocols is available in .docx, .pdf and .epub formats in the IRLib2/manuals/ folder.

**Because this is such an extensive rewrite of IRLib1 and is not backwards compatible, we have made this a separate GitHub repository. The code for IRLib 1.51 is the final version and will no longer be updated or maintained. New users should use this library. Only use the original versions for existing projects that require its original design. In other words IRLib 1.x should be considered depreciated.**

This repository consists of a total of five libraries each of which must be in your arduino/libraries/ folder. So for example it should be installed as follows…

* arduino/libraries/IRLib2
* arduino/libraries/IRLibFreq
* arduino/libraries/IRLibProtocols
* arduino/libraries/IRLibRecv
* arduino/libraries/IRLibRecvPCI

Do not install them in a single folder such as this…

* arduino/libraries/IRLib2_master
  * IRLib2
  * IRLibFreq
  * IRLibProtocols
  * IRLibRecv
  * IRLibRecvPCI
 
Supported platforms include most 8-bit AVR Arduino boards such as Arduino Uno, Leonardo, and Mega 256.
Also supports boards with SAMD 21 processor such as Arduino Zero, Arduino M0 Pro, Adafruit Feather M0, Adafruit Circuit Playground Express, Adafruit Trinket M0, Adafruit Gemma M0, and Arduino MKR 1000. Documentation has been updated to include all new protocols and platforms.
Now supports SAMD51 M4 processor on the Adafruit Metro M4. See SAMD51.txt for details.
