# This file should allow ACE to be built on Windows, using the IAR compiler.

MQX_CPU = PSP_CPU_MK70F120M
MQX_CPU_INC = cortex_m

ifndef MQX_ROOT
  $(error Please set MQX_ROOT to the toplevel directory of the MQX code.)
endif

MQX_CONFIG_COMMON_DIR ?= "$(MQX_ROOT)/config/common"

ifndef MQX_USER_CONFIG_DIR
  $(error Please set MQX_USER_CONFIG_DIR to the directory containing the MQX user_config.h.)
endif

ACE_PLATFORM_CONFIG ?= config-mqx.h
debug ?= 1
optimize ?= 1
threads ?= 1
static_libs_only = 1
gruntimelibselect ?= 1

AR      = iarchive
ARFLAGS = -o
RANLIB  = @true
AR.cc.override = $(RM) $@; echo $(filter %.o, $^) | xargs $(AR) $(ARFLAGS) $@ $(AREXTRA)

fast ?= 0

IAR_PLATFORM ?= arm

CC  = icc$(IAR_PLATFORM)
CXX = icc$(IAR_PLATFORM) --c++

ifndef CXX_VERSION
  CXX_VERSION := $(shell $(CXX) --version)
endif

no_hidden_visibility ?= 1

ifeq ($(inline),0)
  CPPFLAGS += --no_inline
endif

ifeq ($(gruntimelibselect),1)
  CFLAGS += --dlib_config DLib_Config_Normal.h
endif
ifeq ($(gruntimelibselect),2)
  CFLAGS += --dlib_config DLib_Config_Full.h -DACE_MQX_DLIB_FULL
endif
ifeq ($(gruntimelibselect),3)
  CFLAGS += --dlib_config $(DLIB_CONFIG)
endif

CFLAGS += -I$(MQX_CONFIG_COMMON_DIR) -I$(MQX_USER_CONFIG_DIR)
MQX_IO_DIR=$(MQX_ROOT)/mqx/source/io
CFLAGS  += -DMQX_CPU=$(MQX_CPU) --enable_restrict -I$(MQX_ROOT)/mqx/source/include -I$(MQX_ROOT)/mqx/source/psp/$(MQX_CPU_INC) -I$(MQX_ROOT)/mqx/source/psp/$(MQX_CPU_INC)/cpu -I$(MQX_ROOT)/mqx/source/psp/$(MQX_CPU_INC)/compiler/iar -I$(MQX_ROOT)/rtcs/source/include -I$(MQX_ROOT)/mqx/source/bsp/twrk70f120m -I$(MQX_IO_DIR)/adc -I$(MQX_IO_DIR)/asrc -I$(MQX_IO_DIR)/can -I$(MQX_IO_DIR)/cm -I$(MQX_IO_DIR)/core_mutex -I$(MQX_IO_DIR)/dcu4 -I$(MQX_IO_DIR)/debug -I$(MQX_IO_DIR)/dma -I$(MQX_IO_DIR)/enet -I$(MQX_IO_DIR)/esai -I$(MQX_IO_DIR)/esdhc -I$(MQX_IO_DIR)/fbdev -I$(MQX_IO_DIR)/flashx -I$(MQX_IO_DIR)/flextimer -I$(MQX_IO_DIR)/ftm -I$(MQX_IO_DIR)/gpio -I$(MQX_IO_DIR)/hmi -I$(MQX_IO_DIR)/hwtimer -I$(MQX_IO_DIR)/i2c -I$(MQX_IO_DIR)/i2s -I$(MQX_IO_DIR)/imxrt -I$(MQX_IO_DIR)/int_ctrl -I$(MQX_IO_DIR)/io_expander -I$(MQX_IO_DIR)/io_mem -I$(MQX_IO_DIR)/io_null -I$(MQX_IO_DIR)/lcd -I$(MQX_IO_DIR)/lpm -I$(MQX_IO_DIR)/lpm_mcore -I$(MQX_IO_DIR)/lwadc -I$(MQX_IO_DIR)/lwgpio -I$(MQX_IO_DIR)/mcg -I$(MQX_IO_DIR)/mu -I$(MQX_IO_DIR)/nandflash -I$(MQX_IO_DIR)/pcb -I$(MQX_IO_DIR)/pccard -I$(MQX_IO_DIR)/pcflash -I$(MQX_IO_DIR)/pipe -I$(MQX_IO_DIR)/qspi -I$(MQX_IO_DIR)/rnga -I$(MQX_IO_DIR)/rtc -I$(MQX_IO_DIR)/sai -I$(MQX_IO_DIR)/sdcard -I$(MQX_IO_DIR)/sdram -I$(MQX_IO_DIR)/sensor -I$(MQX_IO_DIR)/serial -I$(MQX_IO_DIR)/sim -I$(MQX_IO_DIR)/spi -I$(MQX_IO_DIR)/spi_legacy -I$(MQX_IO_DIR)/spi_slave -I$(MQX_IO_DIR)/tchres -I$(MQX_IO_DIR)/tfs -I$(MQX_IO_DIR)/timer -I$(MQX_IO_DIR)/usb -I$(MQX_IO_DIR)/usb_dcd -I$(MQX_IO_DIR)/gpio/kgpio -I$(MQX_IO_DIR)/sdcard/sdcard_spi -I$(MQX_IO_DIR)/sdcard/sdcard_esdhc -I$(MQX_IO_DIR)/flashx/freescale -I$(MQX_IO_DIR)/enet/macnet -I$(MQX_IO_DIR)/adc/kadc -I$(MQX_ROOT)/mfs/source/include -I$(MQX_ROOT)/posix/source/include

ifeq ($(threads),1)
    CPPFLAGS  += -D_REENTRANT
endif # threads

CCFLAGS += $(CFLAGS)
DCFLAGS += --debug
DLD     = ilinkarm
LD      = ilinkarm

MQX_LIB_DIR ?= $(MQX_ROOT)/lib/twrk70f120m.iar/debug

LDFLAGS += --config $(MQX_LIB_DIR)/bsp/intflash_sramdata.icf --redirect __iar_dlmalloc=malloc \
           --redirect __iar_dlcalloc=calloc --redirect __iar_dlfree=free --manual_dynamic_initialization
SYSTEM_LIBS = -L$(MQX_LIB_DIR)/bsp bsp.a \
              -L$(MQX_LIB_DIR)/psp psp.a \
              -L$(MQX_LIB_DIR)/mfs mfs.a \
              -L$(MQX_LIB_DIR)/rtcs rtcs.a \
              -L$(MQX_LIB_DIR)/posix posix.a

LINK.cc.override = $(PURELINK) $(PRELINK) $(LD) $(LDFLAGS) $(CC_OUTPUT_FLAG) $@ $(SYSTEM_LIBS) $(filter-out %.a,$^) $(VLDLIBS) $(POSTLINK)

OCFLAGS += -O

ifeq ($(optimize),0)
  # Disable all optimizing in code
  CPPFLAGS += -On
endif

SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true
