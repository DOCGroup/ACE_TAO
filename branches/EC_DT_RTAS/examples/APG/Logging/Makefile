# $Id$
#
# Copyright 2003 Addison-Wesley Inc. All Rights Reserved.

BIN =	Simple1 \
	Simple2 \
	Change_Mask \
	Change_Instance_Default \
	Wrap_Macros \
	Wrap_Macros_Alt \
	Trace_Return \
	Use_Stderr \
	Howto_Syslog \
	Use_Syslog \
	Use_Ostream \
	Use_Multiple_Sinks \
	Use_Callback \
	Use_Callback2 \
	Use_Logger \
	Use_Logging_Server \
	Use_LogManager \
	Use_Logging_Strategy

SOURCES = $(addsuffix .cpp,$(BIN))
HEADERS = $(wildcard *.h)

TEXTS = server.conf client.conf logging_strategy.conf

CFLAGS += -DACE_NTRACE=0

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
