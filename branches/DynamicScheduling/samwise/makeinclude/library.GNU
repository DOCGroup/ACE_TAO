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
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU


