#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the SNMP+ACE Release
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB      = libasnmp.a
SHLIB	 = libasnmp.$(SOEXT)

FILES	 = address \
	asn1 \
	counter \
	ctr64 \
	gauge \
	integer \
	octet \
	oid \
	pdu \
	snmp \
	target \
	timetick \
	transaction \
	vb \
        wpdu

TEMPLATE_FILES = 

DEFS    = $(addsuffix .h,$(FILES)) 
DEFS    += $(addsuffix .i,$(FILES))
LSRC    = $(addsuffix .cpp,$(FILES))
#LSRC2   = $(addsuffix .cpp,$(TEMPLATE_FILES))

#### Build the SNMP++ shared library _before_ the static library.
#### This works around an apparent problem with template
#### instantiation with Sun C++ 4.1 and 4.2 (the symptom is link
#### error due to ACE_TimeValue::normalize () not being found).
BUILD	= $(VSHLIB) $(SHLIBA) $(VLIB)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU
 
#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

CCFLAGS += -I$(WRAPPER_ROOT)/ASNMP/

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
