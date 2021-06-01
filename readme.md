# LIN Synchronization break

In LIN implementation, the 15-bit length Synchronization Break is the big issue. The Frame Header ideally would contain, besides the Synchronization Break, the Synch byte 0x55 and the PID field. Here we start with a simple frame structure consisting of Synch Break, Synch Byte (0x55) and three data fields. 