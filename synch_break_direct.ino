/*
 *  2018-03-06. Great!! the screenshot on the scope shows as it MUST period = 670us
 *  Theoreticlly the value should be 645
 *  For further info refer to : Introduction to LIN by Vector, page 22
 */

#include <inttypes.h>
#include "Arduino.h"
#include <HardwareSerial.h> 

#define LIN_SERIAL            HardwareSerial
#define LIN_BREAK_DURATION    15    // Number of bits in the break.
#define LIN_TIMEOUT_IN_FRAMES 2     // Wait this many max frame times before declaring a read timeout.

int serialSpd = 19200;
uint8_t serialOn;   
uint8_t txPin = 1;

 unsigned long int Tbit = 100000/serialSpd;                  // 52.1us
 unsigned long int nominalFrameTime = ((34*Tbit)+90*Tbit);   // 781.5us. On the scope 780us GREAT!!!!!
 unsigned long int timeout = LIN_TIMEOUT_IN_FRAMES * 14 * nominalFrameTime; 


 void serialBreak(void)
{
  if (serialOn) 
  {
    Serial.flush();
    Serial.end();
  }

  pinMode(txPin, OUTPUT);
  digitalWrite(txPin, LOW);  // Send BREAK
  const unsigned long int brkend = (1000000UL/((unsigned long int)serialSpd));
  const unsigned long int brkbegin = brkend*LIN_BREAK_DURATION;
  if (brkbegin > 16383) delay(brkbegin/1000);  // delayMicroseconds unreliable above 16383 see arduino man pages
  else delayMicroseconds(brkbegin);
  
  digitalWrite(txPin, HIGH);  // BREAK delimiter

  if (brkend > 16383) delay(brkend/1000);  // delayMicroseconds unreliable above 16383 see arduino man pages
  else delayMicroseconds(brkend);

  Serial.begin(serialSpd);
  serialOn = 1;
}

void setup()
{
  Serial.begin(19200);
}

void loop()
{
  serialBreak();
}



