#----------------------------------------------------------------------------
#	$Id$
#
#	Nested directory targets makefile definitions
#----------------------------------------------------------------------------

$(TARGETS_NESTED):
	@for dir in $(DIRS) ;\
	do \
		 $(MAKE) -C $$dir $(@:.nested=); \
	done
