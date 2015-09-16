CFLAGS =  -Wall -I../ -std=c99
CFLAGS += -pedantic -std=iso9899:1999
CFLAGS += -Wshadow -Wpointer-arith -Wcast-qual -Wunused-variable
CFLAGS += -Wcast-align -Wstrict-prototypes #-Wconversion

LIBS=

all: UTest test

clean:
	rm -rf *.o utest

test: utest
	./utest

UTest: unity.o main.o unity_fixture.o Dataflash.o testDataflash.o EEPROM.o testEEPROM.o ErrorChecking.o testErrorCheck.o Timing.o  testTiming.o testLogging.o Logging.o Clock.o testClock.o Syslog.o testSyslog.o CmdProcessor.o testCmdProcessor.o ModuleMonitor.o testModMon.o
	gcc -o utest main.o unity.o unity_fixture.o Dataflash.o testDataflash.o EEPROM.o testEEPROM.o ErrorChecking.o testErrorCheck.o Timing.o testTiming.o Logging.o testLogging.o  Clock.o testClock.o Syslog.o testSyslog.o CmdProcessor.o testCmdProcessor.o ModuleMonitor.o testModMon.o

unity.o: unity.c 
	gcc $(CFLAGS) -c unity.c 

unity_fixture.o: unity_fixture.c
	gcc $(CFLAGS) -c unity_fixture.c

main.o: main.c 
	gcc $(CFLAGS) -c main.c
	
testCmdProcessor.o: testCmdProcessor.c CmdProcessor.o
	gcc $(CFLAGS) -c testCmdProcessor.c

CmdProcessor.o: ../CmdProcessor.c ../CmdProcessor.h
	gcc $(CFLAGS) -c ../CmdProcessor.c

testClock.o: testClock.c Clock.o
	gcc $(CFLAGS) -c testClock.c

Clock.o: ../Clock.c ../Clock.h
	gcc $(CFLAGS) -c ../Clock.c

testDataflash.o: testDataflash.c Dataflash.o
	gcc $(CFLAGS) -c testDataflash.c

Dataflash.o: ../Dataflash.c ../Dataflash.h
	gcc $(CFLAGS) -c ../Dataflash.c

testEEPROM.o: testEEPROM.c EEPROM.o
	gcc $(CFLAGS) -c testEEPROM.c

EEPROM.o: ../EEPROM.c ../EEPROM.h
	gcc $(CFLAGS) -c ../EEPROM.c

testErrorCheck.o: testErrorCheck.c ErrorChecking.o
	gcc $(CFLAGS) -c testErrorCheck.c

ErrorChecking.o: ../ErrorChecking.c ../ErrorChecking.h
	gcc $(CFLAGS) -c ../ErrorChecking.c

testLogging.o: testLogging.c Logging.o
	gcc $(CFLAGS) -c testLogging.c

Logging.o: ../Logging.c ../Logging.h
	gcc $(CFLAGS) -c ../Logging.c

ModuleMonitor.o: ../ModuleMonitor.h ../ModuleMonitor.c
	gcc $(CFLAGS) -c ../ModuleMonitor.c

testModMon.o: ModuleMonitor.o testModMon.c
	gcc $(CFLAGS) -c testModMon.c


testSyslog.o: testSyslog.c Syslog.o
	gcc $(CFLAGS) -c testSyslog.c

Syslog.o: ../Syslog.c ../Syslog.h
	gcc $(CFLAGS) -c ../Syslog.c

testTiming.o: testTiming.c Timing.o
	gcc $(CFLAGS) -c testTiming.c

Timing.o: ../Timing.c ../Timing.h
	gcc $(CFLAGS) -c ../Timing.c


