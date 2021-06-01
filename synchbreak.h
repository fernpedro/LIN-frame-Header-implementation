/*
  * Last modified 2018-04-04
    by Pedro Fernandez Acuna
 
  */
 #include <inttypes.h>
 #include "Arduino.h"
 #include <HardwareSerial.h>  
 //#include <SoftwareSerial.h>
 
 #define LIN_SERIAL            HardwareSerial
 #define LIN_BREAK_DURATION    15    // Number of bits in the break.
 #define LIN_TIMEOUT_IN_FRAMES 2     // Wait this many max frame times before declaring a read timeout.
 
 
 class Lin
 {
 protected: 
   void serialBreak(void);
   
 public:
   Lin(LIN_SERIAL& ser=Serial,uint8_t txPin=1);
   LIN_SERIAL& serial;
   uint8_t txPin;               //  what pin # is used to transmit (needed to generate the BREAK signal)
   int     serialSpd;           //  in bits/sec. Also called baud rate
   uint8_t serialOn;            //  whether the serial port is "begin"ed or "end"ed.  Optimization so we don't begin twice.
   unsigned long int timeout;   //  How long to wait for a slave to fully transmit when doing a "read".  You can modify this after calling "begin"
   void begin(int speed);
 
   void send(uint8_t addr, const uint8_t* message,uint8_t nBytes);
 };
   