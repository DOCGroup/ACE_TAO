#----------------------------------------------------------------------------
#       $Id$
#
#       Nested directory targets makefile definitions
#----------------------------------------------------------------------------

$(TARGETS_NESTED):
ifneq ($(DIRS),)
ifeq (Windows,$(findstring Windows,$(OS)))
	@FOR /D %i IN ($(DIRS)) DO $(MAKE) -C %i $(@:.nested=)
else  # ! Windows
	@for dir in $(DIRS); do $(MAKE) -C $$dir $(@:.nested=); done
endif # ! Windows
endif # DIRS
