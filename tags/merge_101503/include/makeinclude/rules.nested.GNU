#----------------------------------------------------------------------------
#       $Id$
#
#       Nested directory targets makefile definitions
#----------------------------------------------------------------------------

# If the client makefile is not called "Makefile", the MAKEFILE
# variable must be set to its actual name before including this
# file to allow the recursive MAKE to work properly.

ifeq ($(MAKEFILE),)
  MAKEFILE = Makefile
endif # ! MAKEFILE

ifeq ($(SUBDIR_MAKEFILE),)
  SUBDIR_MAKEFILE=$(MAKEFILE)
endif

# Make sure that we build directories with DIRS= in sequence instead of in
# parallel, unless DIRS_PARALLEL is set, in which case they may be built
# in parallel if enabled by the proper flags and not disabled elsewhere.

ifeq ($(DIRS_PARALLEL),)
.NOTPARALLEL:
endif

# A rule to build DIR.subdir from DIR by calling MAKE again, with
# DIR as the new directory, SUBDIR_TARGET as the target to build, and
# "Makefile" as the makefile to use in the new directory.

# Triggered by the $(TARGETS_NESTED) target, below.

%.subdir: %
	$(MAKE) -f $(SUBDIR_MAKEFILE) -C $< $(SUBDIR_TARGET)

# Build FOO.nested by calling MAKE again, this time with DIRS (with
# ".subdir" appended to each) as the targets, and passing FOO as the
# real target to build in the SUBDIR_TARGET variable.

$(TARGETS_NESTED):
ifneq ($(DIRS),)
	$(MAKE) -f $(MAKEFILE) SUBDIR_TARGET=$(@:.nested=) $(addsuffix .subdir, $(DIRS))
endif # DIRS
