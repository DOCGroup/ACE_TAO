# $Id$

# platform_openbsd_pthread.GNU

include $(ACE_ROOT)/include/makeinclude/platform_openbsd.GNU

LDFLAGS		+= -pthread
CFLAGS          += -D_THREAD_SAFE
