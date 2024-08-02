ACE_PLATFORM_CONFIG ?= config-emscripten.h
EXEEXT = .js

debug ?= 1
optimize ?= 0
threads ?= 1
inline ?= 1
static_libs_only = 1

CROSS_COMPILE := THIS_VALUE_SHOULD_NOT_BE_USED
override_cross_compile = 1
CC = emcc
CXX = em++
AR = emar
LD = emcc

ifeq ($(threads),1)
  FLAGS_C_CC += -pthread
  LDFLAGS += -pthread
  LIBS += -lrt
endif

ifeq ($(debug),1)
  LDFLAGS += -sASSERTIONS=2
endif

# TODO: Require choosing Filesystem model:
# https://emscripten.org/docs/api_reference/Filesystem-API.html
LDFLAGS += -sNODERAWFS=1

include $(ACE_ROOT)/include/makeinclude/platform_clang_common.GNU
