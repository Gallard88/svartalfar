IMG_NAME=libSvartalfar
LIB_IMAGE=${IMG_NAME}.a

RELEASE=

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
CXX=arm-none-eabi-g++
AR=arm-none-eabi-ar
LD=arm-none-eabi-ld
SIZE=arm-none-eabi-size
RM = rm -rf

CFLAGS = #-mfloat-abi=softfp
CFLAGS += #-mfpu=fpv4-sp-d16
CFLAGS += -fpic -Wall -mcpu=cortex-m3 -mthumb
CFLAGS += -pedantic -std=iso9899:1999
#CFLAGS += -Wshadow -Wpointer-arith -Wcast-qual 
#CFLAGS += -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wconversion
#CFLAGS += -Wtraditional 

ifeq ($(RELEASE), DEBUG)
  CFLAGS += -g3 -O2 
else
  CFLAGS += -g -O2
endif

#CXXFLAGS += -fno-rtti -fno-exceptions -fno-use-cxa-atexit -Wabi -Weffc++

# ATSAM3S4B lib
CFLAGS += -I.

SRCS_S = \

SRCS_C =           \
  Clock.c          \
  CmdProcessor.c   \
  Dataflash.c      \
  EEPROM.c         \
  ErrorChecking.c  \
  Logging.c        \
  Syslog.c         \
  Timing.c 

SRCS_CXX =

OBJS = $(SRCS_S:%.S=%.o)
OBJS += $(SRCS_C:%.c=%.o)
OBJS += $(SRCS_CXX:%.cc=%.o)

all: style tests lib stats

lib: $(LIB_IMAGE) 

$(LIB_IMAGE): $(OBJS)
	$(AR) rcs $(LIB_IMAGE) -o $(OBJS) $(LIBS)

tests: $(OBJS)
	echo "Run Unit tests"

style:
	echo "Styling code"

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -o $(notdir $@) $<

.S.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -o $(notdir $@) $<

.cc.o:
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(DEFINES) -c -o $(notdir $@) $<

clean:
	$(RM) $(notdir $(OBJS)) $(IMG_NAME).sym $(IMG_NAME).lss

stats:
	$(SIZE) $(LIB_IMAGE)

.PHONY: all clean stats
