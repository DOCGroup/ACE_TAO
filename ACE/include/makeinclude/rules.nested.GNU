# -*- Makefile -*-

#----------------------------------------------------------------------------
#       
#
#       Nested directory targets makefile definitions
#----------------------------------------------------------------------------

# If the client makefile is not called "Makefile", the MAKEFILE
# variable must be set to its actual name before including this
# file to allow the recursive MAKE to work properly.

TOP_MAKEFILE := $(word 1,$(MAKEFILE_LIST))
MAKEFILE ?= $(TOP_MAKEFILE)
SUBDIR_MAKEFILE ?= $(MAKEFILE)

# Make sure that we build directories with DIRS= in sequence instead of in
# parallel, unless DIRS_PARALLEL is set, in which case they may be built
# in parallel if enabled by the proper flags and not disabled elsewhere.
DIRS_PARALLEL ?=

ifeq ($(DIRS_PARALLEL),)
.NOTPARALLEL:
endif

# A rule to build DIR.subdir from DIR by calling MAKE again, with
# DIR as the new directory, SUBDIR_TARGET as the target to build, and
# "GNUmakefile" as the makefile to use in the new directory.

# Triggered by the $(TARGETS_NESTED) target, below.

%.subdir: %
ifneq ($(timed),)
	@date=`date`; echo "START: $< $$date"
endif
	cd $< && $(MAKE) -f $(SUBDIR_MAKEFILE) $(SUBDIR_TARGET)
ifneq ($(timed),)
	@date=`date`;echo "FINISH: $< $$date"
endif

# Build FOO.nested by calling MAKE again, this time with DIRS (with
# ".subdir" appended to each) as the targets, and passing FOO as the
# real target to build in the SUBDIR_TARGET variable.

$(TARGETS_NESTED):
ifdef DIRS
  ifneq ($(DIRS),)
	$(MAKE) -f $(MAKEFILE) SUBDIR_TARGET=$(@:.nested=) $(addsuffix .subdir, $(DIRS))
  endif # DIRS
endif # DIRS
