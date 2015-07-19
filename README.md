Svartalfar micro controller framework.
This is a basic framework designed to add basic, common functionality to a project.
It consists of a number of simple, reasonably generic modules.
Modules include:
Clock, basic time keeping functions, conversion between time/date and UNIX style timestamps.
CommandProcessor, a basic CLI terminal. Supports multiple entry points, and can write responses back via a buffer pointer.
Syslog, analogous to the unix syslog(), this is designed to allow modules to use a common reporting system. 
Timing, used as a polled timer. System generates a 1ms timestamp, which this module can use to measure time.

This library requires arm-none-eabi-gcc to compile, at present it is only tested on the Atmel SAM 3 series.
Future targets include all SAM processors, and AVR32, possibly STM32...

