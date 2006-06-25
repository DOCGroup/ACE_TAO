# $Id$

# for OpenVMS GNV

ACE_OPENVMS = 1
versioned_so = 0
ifndef exceptions
  exceptions = 1
endif
ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(optimize))
  optimize = 0
endif

VDIR            = obj/
VSHDIR          = shobj/

LN_S = cp -p
DEFFLAGS        +=
#CCFLAGS         += -D__USE_STD_IOSTREAM -ieee -Wc/template=noauto -Wc/noimplicit
CCFLAGS         += -D__USE_STD_IOSTREAM -ieee -names_as_is_short -msg_disable intoverflow,reftemporary -Wc/template=auto
ifeq ($(debug),1)
LDFLAGS         += -g -threads -Wl/PRELINK=USE_OLB
else
LDFLAGS         += -threads -Wl/PRELINK=USE_OLB
endif
DCCFLAGS        += -g
OCCFLAGS        += -O

CFLAGS          += -ieee
DCFLAGS         += -g
OCFLAGS         += -O

#MATHLIB         = -lm
PIC             =
ARFLAGS         = -c
RANLIB          = @true
SOFLAGS         = -shared -auto_symvec

LIBS            += -lpthread

CC              = cc
CXX             = cxx
LD              = $(CXX)
DLD             = $(CXX)
SOVERSION       =
SOEXT           = exe
EXEEXT          = .exe
INSLIB          = $(ACE_ROOT)/lib
REALCLEAN_FILES += $(CLEANUP_VSHLIB_NO_VER:%.exe=%_symvec.opt) $(CLEANUP_VSHLIB_NO_VER:%.exe=%.DSF) \
	$(CLEANUP_BIN:%=%.DSF)

ifneq ($(MAKEFILE),$(DEPENDENCY_FILE))
VMS_DUMMY_ := $(shell touch $(DEPENDENCY_FILE))
VMS_DUMMY_ := $(VMS_DUMMY_)
endif

