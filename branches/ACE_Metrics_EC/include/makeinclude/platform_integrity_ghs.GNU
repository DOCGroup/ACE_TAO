CROSS-COMPILE = 1

ifndef debug
  debug = 1
endif
ifndef optimize
  optimize = 1
endif
ifndef rtti
  rtti = 0
endif # rtti
shared_libs =
static_libs = 1
ifndef exceptions
  exceptions = 1
endif

#### CPU type
ifndef CPU
  CPU = ppc
endif # CPU

INTEGRITYTARGET = 1

#### Green Hills location, and target-specific definitions.
ifndef GHSROOT
  GHSROOT = /home/scooter2/i40_solaris2
endif

ifndef RTOSROOT
  RTOSROOT = /export/integrity/rtos
endif

ifndef BSP
  BSP = mcp750
endif

CC = $(GHSROOT)/cc$(CPU)
CXX = $(GHSROOT)/cx$(CPU)

# undefine "vector" because it's predefined for 7400 boards
IOPS = -bspname=$(RTOSROOT)/target/$(BSP).bld -os_dir $(RTOSROOT) --one_instantiation_per_object -non_shared -Uvector
ifeq (1, $(exceptions))
   IOPS += --exceptions
   override rtti = 1
endif # exceptions

CCFLAGS  += $(CFLAGS) -integrate -dynamic -I$(RTOSROOT)/INTEGRITY-include-SCA $(IOPS) -bspfile=$(ACE_ROOT)/tests/ACE.bsp $(ACE_ROOT)/tests/INTEGRITY.ld
DCFLAGS  += -G
SOFLAGS  += -shared
DLD      = $(CXX)
LD       = $(CXX)
LDLIBS += -lACE -lshm_client -lnet -livfssca -lposixsca -lsocket
LDFLAGS = -lACE -lshm_client -lnet -livfssca -lposixsca -lsocket
#LDFLAGS  = --no_auto_instantiation
#OCFLAGS  += -OL -OM
PIC      =
AR := $(CXX) -archive $(IOPS)
ARFLAGS = -o


ifeq (0, $(rtti))
  CCFLAGS += --no_rtti
endif # rtti
