#----------------------------------------------------------------------------
#	$Id$
#
#	Nested directory targets makefile definitions
#----------------------------------------------------------------------------

$(TARGETS_NESTED):
ifneq ($(DIRS),)
	@for dir in $(DIRS) ;\
	do \
		 $(MAKE) -C $$dir $(@:.nested=); \
	done
endif # DIRS
