#include "Arduino.h"
#include "synchbreak.h"
#include <util/delay.h>


Lin::Lin(LIN_SERIAL& ser,uint8_t TxPin): serial(ser), txPin(TxPin)
{
}


void Lin::begin(int speed)
{
  serialSpd = speed;
  serial.begin(serialSpd);
  serialOn  = 1;

  unsigned long int Tbit = 100000/serialSpd;  // Not quite in uSec, I'm saving an extra 10 to change a 1.4 (40%) to 14 below
  unsigned long int nominalFrameTime = ((34*Tbit)+90*Tbit);  // 90 = 10*max # payload bytes + checksum (9). 
}

// Generate a BREAK signal (a low signal for longer than a byte) across the serial line
void Lin::serialBreak(void)
{
  if (serialOn)
  {
    serial.flush();
    serial.end();
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

  serial.begin(serialSpd);
  serialOn = 1;
}

/* Send a message across the Lin bus */
void Lin::send(uint8_t addr, const uint8_t* message, uint8_t nBytes)
{
  serialBreak();       // Generate the low signal that exceeds 1 char.
  serial.write(0x55);  // Sync byte
  serial.write(message,nBytes);  // data bytes
  serial.flush();
}

