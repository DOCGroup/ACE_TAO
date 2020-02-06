
static_libs_only=1

include $(ACE_ROOT)/include/makeinclude/platform_macosx_common.GNU

ifneq (,$(XCODE_ROOT))
  XCODE:=$(XCODE_ROOT)
else
  XCODE:=/Applications/Xcode.app
endif

ifndef IPHONE_TARGET
  $(error Please set IPHONE_TARGET to SIMULATOR or HARDWARE) 
endif

ifeq ($(IPHONE_TARGET), SIMULATOR)
  CROSS-COMPILE=1
  IPHONE_PLATFORM:=$(XCODE)/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer
  IPHONE_SDK:=$(IPHONE_PLATFORM)/SDKs/iPhoneSimulator${IPHONE_VERSION}.sdk
  IPHONE_HARDWARE_ARCHITECTURE=x86_64

# June 2017 release, iPhone 6s and later
  CFLAGS  += -miphoneos-version-min=12.0
  CCFLAGS += -miphoneos-version-min=12.0
  LDFLAGS += -miphoneos-version-min=12.0
endif

ifeq ($(IPHONE_TARGET), HARDWARE)
  CROSS-COMPILE=1
  IPHONE_PLATFORM:=$(XCODE)/Contents/Developer/Platforms/iPhoneOS.platform/Developer
  IPHONE_SDK:=$(IPHONE_PLATFORM)/SDKs/iPhoneOS${IPHONE_VERSION}.sdk
# arm64 iPhone 6 and later
  IPHONE_HARDWARE_ARCHITECTURE=arm64

ifeq ($(iphone_bitcode),1)
# enable bitcode
  CFLAGS  += -fembed-bitcode
  CCFLAGS += -fembed-bitcode
endif

ifeq ($(iphone_codesign),1)
  CODESIGN_ALLOCATE:=$(IPHONE_PLATFORM)/usr/bin/codesign_allocate
  POSTLINK=; codesign -f -s "iPhone Developer" $(BIN)
endif
  rwho = 0
endif

# pass a flag to the preprocessor
CFLAGS  += -DACE_HAS_IOS
CCFLAGS += -DACE_HAS_IOS

CC:=$(XCODE)/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
CXX:=$(XCODE)/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++
AR:=$(XCODE)/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar
ARFLAGS = rSv
RANLIB:=$(XCODE)/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib -no_warning_for_no_symbols
DLD = $(CXX)
LD  = $(CXX)
CFLAGS  += -arch $(IPHONE_HARDWARE_ARCHITECTURE) -isysroot $(IPHONE_SDK)
CCFLAGS += -arch $(IPHONE_HARDWARE_ARCHITECTURE) -isysroot $(IPHONE_SDK)

ifneq (,$(HOST_ROOT))
  TAO_IDLFLAGS += -g $(HOST_ROOT)/bin/ace_gperf
  TAO_IDL = $(HOST_ROOT)/bin/tao_idl
  TAO_IDL3_TO_IDL2 = $(HOST_ROOT)/bin/tao_idl3_to_idl2
  TAO_IDL_DEP = $(TAO_IDL)
  TAO_IDL3_TO_IDL2_DEP = $(TAO_IDL3_TO_IDL2)
  # make sure to use the target compiler, not the cross-compiler
  # as preprocessor for the cross-compiled idl tools
  TAO_IDL_PREPROCESSOR = clang
endif

