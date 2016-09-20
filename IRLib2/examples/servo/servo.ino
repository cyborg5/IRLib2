/* servo.ino Example sketch for IRLib2
 *  Illustrate how to now to control a servo using IR signals.
 */
#include <IRLibDecodeBase.h> // First include the decode base
#include <IRLib_P02_Sony.h>  // Include only the protocol you are using

IRdecodeSony myDecoder; // Now declare an instance of that decoder.

#include <Servo.h> 
/* Note: Servo library uses TIMER1. The default timer for IRLibRecv 
 *  on Arduino Uno is TIMER2 so there is no conflict. However a 
 *  default timer on Arduino Leonardo is TIMER1 so you Have the choice 
 *  of either will have to switch to IRLibRecvPCI or IRLibRecvLoop
 *  to avoid the conflict. Alternatively you can modify the timer used 
 *  to use TIMER3 or TIMER4 as specified in IRLibHardware.h. Also you 
 *  will need to modify the input being used.
 */

#include <IRLibRecv.h> 
IRrecv myReceiver(2); //pin number for the receiver

Servo myServo;  // create servo object to control a servo 
int16_t pos;         // variable to store the servo position 
int16_t speed;       // Number of degrees to move each time a left/right button is pressed
 
void setup() { 
  myServo.attach(9);      // attaches the servo on pin 9 to the servo object 
  pos = 90;               // start at midpoint 90 degrees
  speed = 3;              // servo moves 3 degrees each time left/right is pushed
  myServo.write(pos);     // Set initial position
  myReceiver.enableIRIn(); // Start the receiver
} 
 
// You will have to set these values depending on the protocol
// and remote codes that you are using. These are from my Sony DVD/VCR
#define MYPROTOCOL SONY
#define RIGHT_ARROW   0x86bca //Move several clockwise
#define LEFT_ARROW    0x46bca //Move servo counterclockwise
#define SELECT_BUTTON 0xd0bca //Center the servo
#define UP_ARROW      0x42bca //Increased number of degrees servo moves
#define DOWN_ARROW    0xc2bca //Decrease number of degrees servo moves
#define BUTTON_0 0x90bca  //Pushing buttons 0-9 moves to fix positions
#define BUTTON_1 0x00bca  // each 20 degrees greater
#define BUTTON_2 0x80bca
#define BUTTON_3 0x40bca
#define BUTTON_4 0xc0bca
#define BUTTON_5 0x20bca
#define BUTTON_6 0xa0bca
#define BUTTON_7 0x60bca
#define BUTTON_8 0xe0bca
#define BUTTON_9 0x10bca

void loop() { 
  if (myReceiver.getResults()) {
    if(myDecoder.decode()) {
      switch(myDecoder.value) {
        case LEFT_ARROW:    pos=min(180,pos+speed); break;
        case RIGHT_ARROW:   pos=max(0,pos-speed); break;
        case SELECT_BUTTON: pos=90; break;
        case UP_ARROW:      speed=min(10, speed+1); break;
        case DOWN_ARROW:    speed=max(1, speed-1); break;
        case BUTTON_0:      pos=0*20; break;
        case BUTTON_1:      pos=1*20; break;
        case BUTTON_2:      pos=2*20; break;
        case BUTTON_3:      pos=3*20; break;
        case BUTTON_4:      pos=4*20; break;
        case BUTTON_5:      pos=5*20; break;
        case BUTTON_6:      pos=6*20; break;
        case BUTTON_7:      pos=7*20; break;
        case BUTTON_8:      pos=8*20; break;
        case BUTTON_9:      pos=9*20; break;
      }
      myServo.write(pos); // tell servo to go to position in variable 'pos' 
    }
    myReceiver.enableIRIn();
  }
}

