# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the GNU compiler.

# Of late, this platform and compiler combination hasn't been tested. In
# fact neither compilation nor testing has been done for almost a year.
# So it's better to avoid this combination, if possible.

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_pre.GNU

# Include the macro file for osf1_4.x
#
include $(ACE_ROOT)/include/makeinclude/platform_osf1_4.x_g++.GNU

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_post.GNU
