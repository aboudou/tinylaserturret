/*
 * Nunchuck functions  -- Talk to a Wii Nunchuck
 * 
 * Adapted to work with TinyWire and ATtiny85 by Arnaud Boudou
 *
 * This library is from the Bionic Arduino course : 
 *                          http://todbot.com/blog/bionicarduino/
 *
 * 2007-11 Tod E. Kurt, http://todbot.com/blog/
 *
 * The Wii Nunchuck reading code originally from Windmeadow Labs
 *   http://www.windmeadow.com/node/42
 */

#include "TinyWireM.h"

static uint8_t nunchuck_buf[6];   // array to store nunchuck data,

// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
static void nunchuck_init()
{ 
    TinyWireM.begin();                // join i2c bus as master
    TinyWireM.beginTransmission(0x52);// transmit to device 0x52
    TinyWireM.send((uint8_t)0x40);// sends memory address
    TinyWireM.send((uint8_t)0x00);// sends sent a zero.  
    TinyWireM.endTransmission();// stop transmitting
}

// Send a request for data to the nunchuck
// was "send_zero()"
static void nunchuck_send_request()
{
    TinyWireM.beginTransmission(0x52);// transmit to device 0x52
    TinyWireM.send((uint8_t)0x00);// sends one byte
    TinyWireM.endTransmission();// stop transmitting
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
static char nunchuk_decode_byte (char x)
{
    x = (x ^ 0x17) + 0x17;
    return x;
}

// Receive data back from the nunchuck, 
// returns 1 on successful read. returns 0 on failure
static int nunchuck_get_data()
{
    int cnt=0;
    TinyWireM.requestFrom (0x52, 6);// request data from nunchuck
    while (TinyWireM.available ()) {
        // receive byte as an integer
        nunchuck_buf[cnt] = nunchuk_decode_byte( TinyWireM.receive() );
        cnt++;
    }
    nunchuck_send_request();  // send request for next data payload
    // If we recieved the 6 bytes, then go print them
    if (cnt >= 5) {
        return 1;   // success
    }
    return 0; //failure
}


// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_zbutton()
{
    return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1;  // voodoo
}

// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_cbutton()
{
    return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;  // voodoo
}

// returns value of x-axis joystick
static int nunchuck_joyx()
{
    return nunchuck_buf[0]; 
}

// returns value of y-axis joystick
static int nunchuck_joyy()
{
    return nunchuck_buf[1];
}

// returns value of x-axis accelerometer
static int nunchuck_accelx()
{
    return nunchuck_buf[2];   // FIXME: this leaves out 2-bits of the data
}

// returns value of y-axis accelerometer
static int nunchuck_accely()
{
    return nunchuck_buf[3];   // FIXME: this leaves out 2-bits of the data
}

// returns value of z-axis accelerometer
static int nunchuck_accelz()
{
    return nunchuck_buf[4];   // FIXME: this leaves out 2-bits of the data
}
