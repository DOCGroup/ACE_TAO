#----------------------------------------------------------------------------
#	$Id$
#
#	Nested directory targets makefile definitions
#----------------------------------------------------------------------------

$(TARGETS_NESTED):
	@for dir in $(DIRS) ;\
	do \
		(echo " ========> Making $(@:.nested=): `pwd`"; \
		 $(MAKE) -C $$dir $(@:.nested=); \
		 echo "<========     End $(@:.nested=): `pwd`") ; \
	done
