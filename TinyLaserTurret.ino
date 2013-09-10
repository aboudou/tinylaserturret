#include "TinyWireM.h"        // I2C library for ATtiny AVR
#include "nunchuck_funcs.h"   // Wii Nunchuck helper functions
#include "Servo8Bit.h"        // Servo library for ATtiny AVR

// Useful pins definitions
#define PANSERVOPIN 4         // Pan servo signal pin
#define TILTSERVOPIN 3        // Tilt servo signal pin
#define LASERPIN 1            // Laser module signal pin

// X axis values for nunchuck's joystick. You may adapt these for your own nunchuck
#define JOYXLEFT   34         // X Axis - Full left value
#define JOYXCENTER 133        // X Axis - Center value
#define JOYXRIGHT  230        // X Axis - Full right value

// Y axis values for nunchuck's joystick. You may adapt these for your own nunchuck
#define JOYYDOWN   36         // Y Axis - Full down value
#define JOYYCENTER 128        // Y Axis - Center value
#define JOYYUP     220        // Y Axis - Full up value

// Pan servo angle values. Adapt to fit your needs.
// You may need to swap left and right values depending on servo position.
#define PANSERVOLEFT    125   // Pan servo - Full left angle
#define PANSERVOCENTER  90    // Pan servo - Center angle
#define PANSERVORIGHT   55    // Pan servo - Full right angle

// Tilt servo angle values. Adapt to fit your needs.
// You may need to swap down and up values depending on servo position.
#define TILTSERVODOWN    55   // Tilt servo - Full down angle
#define TILTSERVOCENTER  90   // Tilt servo - Center angle
#define TILTSERVOUP      125  // Tilt servo - Full up angle


// Nunchuck useful variables
byte joyx, joyy, zbut;

// Servo declaration
Servo8Bit panServo;
Servo8Bit tiltServo;

void setup() {
  // Servo init
  panServo.attach(PANSERVOPIN);
  tiltServo.attach(TILTSERVOPIN);
  
  // Center servo
  panServo.write(PANSERVOCENTER);
  tiltServo.write(TILTSERVOCENTER);
  
  // Nunchuck init
  nunchuck_init();
  
  // Laser pin init
  pinMode(LASERPIN, OUTPUT);
  digitalWrite(LASERPIN, LOW);
  
}

void loop() {
  // Get nunchuck data
  nunchuck_get_data();
  
  joyx = nunchuck_joyx();
  joyy = nunchuck_joyy();
  zbut = nunchuck_zbutton();
  
  // Switch laser ON or OFF
  if (zbut == 1) {
    digitalWrite(LASERPIN, HIGH);
  } else {
    digitalWrite(LASERPIN, LOW);
  }
  
  
  // Move pan servo
  if (joyx == JOYXCENTER) {
    panServo.write(PANSERVOCENTER);
  } else if (joyx < JOYXCENTER) {
    byte position = map(joyx, JOYXLEFT, JOYXCENTER, PANSERVOLEFT, PANSERVOCENTER);
    panServo.write(position);
  } else {
    byte position = map(joyx, JOYXCENTER, JOYXRIGHT, PANSERVOCENTER, PANSERVORIGHT);
    panServo.write(position);
  }

  // Move tilt servo
  if (joyy == JOYYCENTER) {
    tiltServo.write(TILTSERVOCENTER);
  } else if (joyy < JOYYCENTER) {
    byte position = map(joyy, JOYYDOWN, JOYYCENTER, TILTSERVOUP, TILTSERVOCENTER);
    tiltServo.write(position);
  } else {
    byte position = map(joyy, JOYYCENTER, JOYYUP, TILTSERVOCENTER, TILTSERVODOWN);
    tiltServo.write(position);
  }
  
  delay(10);
}
