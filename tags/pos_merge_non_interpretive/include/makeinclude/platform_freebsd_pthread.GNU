# $Id$

# platform_freebsd_pthread.GNU

include $(ACE_ROOT)/include/makeinclude/platform_freebsd.GNU

LDFLAGS		+= -pthread
CFLAGS          += -D_THREAD_SAFE
