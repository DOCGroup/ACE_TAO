# $Id$
# VxWorks 5.5.x (x > 0).

#### Notes:
#### 1) This file requires that the WIND_BASE and WIND_HOST_TYPE environment
####    variables be set.  If the target CPU is not a PPC604, then your CPU
####    environment variable must be set.  If perl is not on your path,
####    you'll also need to set your PERL_PATH environment variable to
####    the full path to perl.
#### 2) If you have problems with munch output not being compilable
####    because it contains identifiers with ".", e.g., ".cpp", in them:
####    add a global variable or function to that .cpp file.  See
####    ace/IOStream_T.cpp for an explanation and example.

VXWORKS = 1
CROSS-COMPILE = 1

ifeq ("$(OS)", "Windows_NT")
vxworks_ntbuild = 1
endif

ifndef debug
  debug = 1
endif # ! debug
ifndef optimize
  optimize = 1
endif # ! optimize
ifndef rtti
  rtti = 1
endif # ! rtti
ifndef threads
  threads = 1
endif

ifndef exceptions
  exceptions = 0
endif

shared_libs =
static_libs = 1
# VxWorks doesn't have rwho
rwho = 0

ifeq (,$(WIND_BASE))
  default:
	@ERROR: you must set your WIND_BASE environment variable
endif # WIND_BASE

ifeq (,$(WIND_HOST_TYPE))
  default:
	@ERROR: you must set your WIND_HOST_TYPE environment variable
endif # WIND_HOST_TYPE

ifeq ($(DIAB_HOST_TYPE),)
ifeq ("$(OS)", "Windows_NT")
  DIAB_HOST_TYPE = WIN32
else
  DIAB_HOST_TYPE = UNIX
endif
endif

HOST_DIR = $(WIND_BASE)/host/$(WIND_HOST_TYPE)

ifeq (,$(PERL_PATH))
  PERL_PATH = perl
endif # ! PERL_PATH

ifeq ($(VXWORKS_VERSION_FLAG),)
  VXWORKS_VERSION_FLAG = -DACE_VXWORKS=0x551
endif # VXWORKS_VERSION_FLAG

ifeq ($(TOOL),)
  override TOOL = diab
else
  override TOOL := $(TOOL)
endif
override TOOL_FAMILY := $(TOOL)

ifeq ("$(TOOL)","gnu")
ifndef templates
  templates = automatic
endif # ! templates
endif

PRJ_TYPE = vxApp
TGT_DIR = $(WIND_BASE)/target

ifeq ($(CPU),)
  # default CPU
  CPU = SIMNT
else
  override CPU := $(CPU)
endif

ifeq ($(shared_libs),1)
  SHARED_LIBS = 1
endif
ifeq ($(shared_libs_only),1)
  SHARED_LIBS = 1
endif

ifeq ($(SHARED_LIBS),1)
  ACELIB = -L./
  #### There's no difference between non-shared and shared object code.
  VSHDIR = $(VDIR)
endif # shared_libs

BIN_UNCHECKED ?=

ifneq ("$(BIN_UNCHECKED)","")
  ## set variables for VxWorks linktarget identification
  PRJ_FILE = $(basename $(notdir $(BIN_UNCHECKED))).vxworks
  EXEEXT = .out
  PRJ_DIR = .
  VXWORKSLINK = true
  GENERATED_DIRTY += $(PRJ_DIR)/$(PRJ_FILE)
endif

include $(TGT_DIR)/h/make/defs.default
##include $(TGT_DIR)/h/make/defs.link
include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)
include $(TGT_DIR)/h/make/defs.$(PRJ_TYPE)
include $(TGT_DIR)/h/tool/$(TOOL_FAMILY)/make.$(CPU)$(TOOL)

ifeq ("$(VXWORKSLINK)","true")
  override LARGE_PROJECT =
  PRJ_OBJS = $(addprefix $(VDIR), $(OBJS))
  LD_PARTIAL += -L$(ACE_ROOT)/ace -L./ $(LDLIBPATH) $(LD_LINK_PATH)
  PRJ_OBJS_FOR_LD_PARTIAL=$(addprefix $(VDIR), $(OBJS)) $(VLDLIBS)
  PRJ_OBJS_FOR_LD=$(addprefix $(VDIR), $(OBJS))
  PRJ_OBJS_FOR_NM=$(addprefix $(VDIR), $(OBJS))
  PRJ_OBJS_FOR_AR=$(addprefix $(VDIR), $(OBJS))
  POST_BUILD_RULE = $(POSTLINK)
  COMPILE_TRADITIONAL = $(CC) $(OPTION_OBJECT_ONLY) $(OPTION_DOLLAR_SYMBOLS)\
			  $(filter-out -ansi, $(CFLAGS))

  ## make sure default target stays the ACE/TAO default target
  ##
default: all

  ## dummy target to satify VxWorks dependencies
$(PRJ_DIR)/$(PRJ_FILE):
	@echo 1>$@
endif

ifneq ($(static_libs),1)
  AR             := echo
  ARFLAGS        := "build static lib only"
endif # ! static_libs

CPPFLAGS += $(VXWORKS_VERSION_FLAG)

ifeq ($(debug),1)
  DCFLAGS += -g
  DCCFLAGS += -g
  CPPFLAGS +=
endif

ifeq ("$(TOOL)","gnu")
# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

ifeq ("$(templates)","automatic")
  C++FLAGS += -fmerge-templates
endif

# TEMPLATES_FLAG is set by platform_g++_common.GNU
ifneq ($(TEMPLATES_FLAG),) # If specified, add it to the CCFLAGS
  CPPFLAGS += $(TEMPLATES_FLAG)
endif
endif  # TOOl == gnu

ifeq ("$(TOOL)","diab")
ifeq ($(exceptions),1)
  C++FLAGS += -Xexceptions
else
  C++FLAGS += -Xexceptions-off
endif
else
ifeq ("$(TOOL)","gnu")
ifeq ($(exceptions),1)
  C++FLAGS += -fexceptions
else
  C++FLAGS += -fno-exceptions
endif
endif
endif

ifeq (0,$(rtti))
  #### Don't use RTTI, because Tornado 1.0.x libraries don't support it.
ifeq ("$(TOOL)","gnu")
  C++FLAGS += -fno-rtti -DACE_LACKS_RTTI
else
  C++FLAGS += -Xrtti-off -DACE_LACKS_RTTI
endif
endif # rtti

ifneq ($(findstring $(VX_CPU_FAMILY),ppc arm),)
ifeq ("$(TOOL)","diab")
  C++FLAGS += -Xchar-signed
else
ifeq ("$(TOOL)","gnu")
  C++FLAGS += -fsigned-char
endif
endif
endif

ifneq ($(optimize),1)
  CC_OPTIM_NORMAL =
endif

CCFLAGS         += $(C++FLAGS)

PIC              =
