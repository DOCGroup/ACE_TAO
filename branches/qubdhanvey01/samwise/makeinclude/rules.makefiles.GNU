#
# $Id$
#

$(TARGETS_MAKEFILES):
ifneq ($(MAKEFILES),)
  ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MAKEFILES)) DO $(MAKE) -f %m MAKEFILES= MAKEFILE=$$m $(@:.makefiles=)"
  else  # ! Windows
	@for m in $(MAKEFILES); do \
        $(MAKE) -f $$m MAKEFILES= MAKEFILE=$$m $(@:.makefiles=); \
        done
  endif # ! Windows
endif # MAKEFILES

# ifneq ($(MAKEFILES),)
# $(TARGETS_MAKEFILES): %: $(foreach mk, $(MAKEFILES), %.$(mk:.gnu=))
#
# $(foreach target, $(TARGETS_MAKEFILES), $(foreach mk, $(MAKEFILES), $(target).$(mk:.gnu=))):
#	$(MAKE) MAKEFILE=$(patsubst .%,%,$(suffix $@).gnu) \
#	        MAKEFILES= -f $(patsubst .%,%,$(suffix $@).gnu) \
#		$(subst .makefiles,,$(basename $@))
# endif # MAKEFILES


