# $Id$
# pSOS with Trimedia compiler

####
#### Notes: 
####
####   1) This file assumes that the PSS_ROOT environment variable is set.
####

TCS   	    = /net/neptune/neptune/n3/jin
#TCS        = /net/video/video/v0/dtv_stage1/TCS/SunOS
HOST        = tmsim
ENDIAN      = el

PSS_ROOT    = $(TCS)/OS/pSOS/pSOSystem
PSS_SYS_DIR	= $(PSS_ROOT)/sys/os
PSS_SYS_LIBS	= $(PSS_SYS_DIR)/psos_tm_$(ENDIAN).o 

PSOS_SYSTEM = $(TCS)/OS/pSOS/pSOSystem
PSOS_DEFS   = -DSC_PSOS=YES -DSC_PSOSM=NO -DSC_PNA=NO -D__sparc 
CC          = $(TCS)/bin/tmcc -$(ENDIAN)  -host $(HOST) $(PSOS_DEFS)
CXX         = $(TCS)/bin/tmCC -$(ENDIAN)  -host $(HOST) $(PSOS_DEFS)
CINCS       = -I. -I$(PSOS_SYSTEM)/include
#-I$(TCS)/include/$(HOST)
CFLAGS      = $(CINCS) 
CCFLAGS     = $(CINCS)

AR          = $(TCS)/bin/tmar
ARFLAGS     = rcv
LD          = $(TCS)/bin/tmld
LDFLAGS     = -bremoveunusedcode -bcompact -bfoldcode -ldev_g

RANLIB      = echo

# -w suppresses warnings 
# -H tells you which header files are being included 
# (switch between these)
#DCFLAGS         += -g
#DCFLAGS         += -g -H
#DCFLAGS         += -g -H -w
#DCFLAGS          += -g -w

LIBS            += $(PSS_SYS_LIBS)  

#INCLDIRS        += -I$(PSS_ROOT)/include -I$(PSS_BSP) -I$(ACE_ROOT)/ace


