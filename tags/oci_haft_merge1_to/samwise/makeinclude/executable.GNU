#
# $Id$
#

DEPENDENCY_FILE=.$(MAKEFILE).depend

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
-include $(ACE_ROOT)/TAO/rules.tao.GNU
-include $(ACE_ROOT)/TAO/taoconfig.mk
include $(ACE_ROOT)/samwise/makeinclude/macros.GNU
include $(ACE_ROOT)/samwise/makeinclude/rules.common.GNU
include $(ACE_ROOT)/samwise/makeinclude/rules.nomakefiles.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU

VOBJS = \
  $(subst .cpp,.$(OBJEXT),$(foreach file,$(SRC),$(VDIR)$(notdir $(file))))

ifeq (1,$(using_aix_vacpp))
$(BIN): %: $(SRC)
	vacbld_r $< -severitylevel=warning
else
$(BIN): %: $(VOBJS)
	$(LINK.cc) $(CC_OUTPUT_FLAG) $@ $^ $(LDFLAGS) $(VLDLIBS) $(POSTLINK)
endif

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
