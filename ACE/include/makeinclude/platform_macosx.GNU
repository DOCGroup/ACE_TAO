# include the platform_macosx_*.GNU based on the detected MacOS version

MACOS_RELEASE_VERSION=$(shell sw_vers -productVersion)
MACOS_REL_WORDS := $(subst ., ,${MACOS_RELEASE_VERSION})
MACOS_MAJOR_VERSION = $(word 1,${MACOS_REL_WORDS})
MACOS_MINOR_VERSION = $(word 2,${MACOS_REL_WORDS})
MACOS_BUILD_VERSION = $(word 3,${MACOS_REL_WORDS})


MACOS_CODENAME_VER_10_2   :=
MACOS_CODENAME_VER_10_3   := panther
MACOS_CODENAME_VER_10_4   := tiger
MACOS_CODENAME_VER_10_5   := leopard
MACOS_CODENAME_VER_10_6   := snowleopard
MACOS_CODENAME_VER_10_7   := lion
MACOS_CODENAME_VER_10_8   := mountainlion
MACOS_CODENAME_VER_10_9   := mavericks
MACOS_CODENAME_VER_10_10  := yosemite
MACOS_CODENAME_VER_10_11  := elcapitan
MACOS_CODENAME_VER_10_12  := sierra
MACOS_CODENAME_VER_10_13  := highsierra
MACOS_CODENAME_VER_latest := highsierra

MACOS_CODENAME = $(MACOS_CODENAME_VER_$(MACOS_MAJOR_VERSION)_$(MACOS_MINOR_VERSION))

ifeq ($(MACOS_MAJOR_VERSION),10)
  ifeq ($(shell test $(MACOS_MINOR_VERSION) -gt 13; echo $$?),0)
    ## if the detected version is greater than the latest know version,
    ## just use the latest known version
    MACOS_CODENAME = $(MACOS_CODENAME_VER_latest)
  else ifeq ($(shell test $(MACOS_MINOR_VERSION) -lt 2; echo $$?),0)
    ## Unsupported minor version
    $(error Unsupported MacOS version $(MACOS_RELEASE_VERSION))
  endif
else
  ## Unsupported major version
  $(error Unsupported MacOS version $(MACOS_RELEASE_VERSION))
endif

include $(ACE_ROOT)/include/makeinclude/platform_macosx_$(MACOS_CODENAME).GNU

