# -*- Makefile -*-
# VxWorks 7

# Building ACE for VxWorks 7
#
# Unlike earlier versions, VxWorks 7 requires a VSB before application code
# can be compiled (either kernel mode or RTP).  See the VxWorks documentation
# for information on creating a VSB (VxWorks Source Build).
#
# Create the file ace/config.h containing #include "ace/config-vxworks.h"
# Create the file include/makeinclude/platform_macros.GNU containing...
# VSB_DIR = <<absolute path to the VSB directory for the target system>>
# TOOL = gnu
# HOST_ROOT = <<location of host tools if using tao_idl, etc.>>
# << add other build settings here (debug, optimize, inline, rtp) >>
# include $(ACE_ROOT)/include/makeinclude/platform_vxworks.GNU
#
# Use the wrenv script for your host system to start a VxWorks development shell
# and build ACE from that shell using the normal procedure (set ACE_ROOT,
# generate makefiles if necessary, etc.).
#
# Not all configurations supported on VxWorks 6.x have been brought forward
# to this file.  If you are building a configuration that has not yet been
# attempted on VxWorks 7, this file may need to be changed.
VXWORKS = 1
CROSS-COMPILE = 1

debug ?= 1
optimize ?= 1
threads ?= 1
rtp ?= 1
pthread ?= $(if $(findstring 1,$(rtp)),1,0)
xerces ?= 0
aio ?= 0
versioned_so ?= 0
footprint ?= 0
rwho = 0
templates ?= automatic
build_tao_idl_be = 0

ifeq (,$(WIND_BASE))
  default:
  @ERROR: you must set your WIND_BASE environment variable
endif # WIND_BASE

ifeq (,$(WIND_HOST_TYPE))
  default:
  @ERROR: you must set your WIND_HOST_TYPE environment variable
endif # WIND_HOST_TYPE

ifeq ($(WIND_HOST_TYPE),x86-win32)
  vxworks_ntbuild ?= 0
  mingw32 ?= 1
  lacks_touch ?= 0
  PWD = $(subst \,/,$(shell pwd))
  ACE_ROOT := $(subst \,/,$(ACE_ROOT))
  TAO_ROOT := $(subst \,/,$(TAO_ROOT))
  HOST_ROOT := $(subst \,/,$(HOST_ROOT))
  override RM = rm -f
  HOST_EXE_EXT = .exe
endif # x86-win32

ifneq (,$(HOST_ROOT))
  TAO_IDLFLAGS += -g $(HOST_ROOT)/bin/ace_gperf
  TAO_IDL = $(HOST_ROOT)/bin/tao_idl$(HOST_EXE_EXT)
  TAO_IDL_DEP = $(TAO_IDL)
else
  # Append the HOST_EXE_EXT if it's not already there
  TAO_IDL_DEP = $(TAO_IDL:$(HOST_EXE_EXT)=)$(HOST_EXE_EXT)
endif

VXWORKS_VERSION_FLAG ?= -DACE_VXWORKS=0x700
CPPFLAGS += $(VXWORKS_VERSION_FLAG)

ifneq ($(WIND_RELEASE_ID), )
  CPPFLAGS += -DACE_VXWORKS_RELEASE=0x$(subst .,,$(WIND_RELEASE_ID))
endif

ifeq ($(pthread),1)
  CPPFLAGS += -DACE_HAS_PTHREADS
endif

ifeq ($(rtp),0)
  VSHDIR = $(VDIR)

  ifneq ($(shared_libs_only),1)
    DKM_LIBS = $(if $(findstring 1,$(static_libs)$(static_libs_only)),$(LDLIBS))
  endif

  IFILE = $(VDIR)$(notdir $(basename $(1)))
  define LINK.cc.override
$(LD) -r -nostdlib $(LDFLAGS) -o $(call IFILE,$@)_partial.o $(filter %.o,$^) $(DKM_LIBS)
	$(NM) $(call IFILE,$@)_partial.o | $(MUNCH) > $(call IFILE,$@)_ctdt.c
	$(COMPILE.c) -o $(call IFILE,$@)_ctdt.o $(call IFILE,$@)_ctdt.c
	$(LD) -r -nostdlib -X $(LD_SCRIPT_DOWNLOAD) -o $@ $(call IFILE,$@)_ctdt.o $(call IFILE,$@)_partial.o $(DKM_FINAL_LIBS)
  endef

  SOLINK.cc.override = $(LINK.cc.override)
  CLEANUP_OBJS += $(foreach x,_ctdt.c _ctdt.o _partial.o,$(addsuffix $(x),$(VDIR)$(notdir $(basename $(if $(LIB_NAME),$(VLIB),$(VBIN))))))
endif

ace_vx_mk_dir = $(if $(findstring 1,$(rtp)),$(WIND_USR_MK),$(WIND_KRNL_MK))
include $(ace_vx_mk_dir)/defs.library.mk

ifneq ($(rtp),0)
  ## If we are building rtp and using the llvm (clang) compiler, we need to
  ## use the linker directly to create shared libraries and executables.
  ifeq ($(TOOL),llvm)
    LLD = $(__WRS_TOOL_PATH)/ld$(TOOLENV)
    define LINK.cc.override
$(LLD) $(LDFLAGS) $(LD_EMULATION) --allow-shlib-undefined -EL $(VSB_DIR)/usr/lib/common/crt0.o -o $@ $(filter %.o,$^) --start-group --as-needed $(LDLIBS) -lc -lc_internal -lllvm -lcplusplus -lllvmcplus -lnet -ldl --end-group
    endef
    define SOLINK.cc.override
$(LLD) $(LDFLAGS) $(LD_EMULATION) -shared --exclude-libs libc_internal.a -u __init -u __fini -o $@ $(filter %.o,$^) --start-group --as-needed $(ACE_SHLIBS) -lc -lc_internal -lcplusplus -lnet --end-group
    endef
  endif
endif

dkm_libstdcpp ?= $(if $(findstring y,$(_WRS_CONFIG_PRI_diab)),1,0)
ifeq ($(dkm_libstdcpp),1)
  DKM_LIBS += -L$(VSB_DIR)/krnl/gnu_standard --start-group -lstl -lgnucplus --end-group
  DKM_FINAL_LIBS = -L$(VSB_DIR)/krnl/gnu_standard -lgnucplus
endif

CCFLAGS += $(C++FLAGS)
PIC += $(OPTION_SHARED_CODE)

OCFLAGS += $(CC_OPTIM_NORMAL)
OCCFLAGS += $(CC_OPTIM_NORMAL)
CC_OPTIM =

DCFLAGS += -g
DCCFLAGS += -g

DLD = $(LD)
LDFLAGS += $(LDFLAGS_COMMON)

EXEEXT = $(if $(findstring 1,$(rtp)),.vxe,.out)

SO_NAME = $(SHLIB)
SOFLAGS += $(C++_COMPILER) $(OPTION_SHARED_CODE) $(LD_SHARED_LIB) \
           $(LD_OPTION_SONAME)

ifeq ($(rtp),1)
  LD = $(CXX)
  LDFLAGS := $(filter-out -X -N,$(LDFLAGS))
  LDFLAGS += -L$(VSB_DIR)/usr/lib/common$(if $(filter library,$(PRJ_TYPE)),/PIC -L$(VSB_DIR)/usr/lib/common)
endif

LIBS_CPLUSPLUS = $(foreach lib,$(LIBCPLUS), -l$(lib))
LIBS += $(LIBS_CPLUSPLUS)

build_nonstatic = $(if $(findstring 1,$(shared_libs)$(shared_libs_only)),$(if $(filter 1,$(static_libs_only)),,1))
LDFLAGS += $(if $(build_nonstatic),$(LDFLAGS_DYNAMIC),-static $(LDFLAGS_STATIC))
