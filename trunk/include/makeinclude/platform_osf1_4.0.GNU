# $Id$

# This platform macros file is intended to work with 
# Digital UNIX 4.0 (OSF/1 4.0).

debug = 1

CC              = cxx
CXX             = $(CC)
CFLAGS          +=

#### These flags can only be used with cxx Version 6.0 and later.
# WARNING_FLAGS +=
# 9:   nested comment not allowed, but there's one in /usr/include/pdsc.h
# 174: expr_has_no_effect
# 193: zero used for undefined preprocessing identifier
# 236: controlling expression is constant
# 340: value copied to temporary, reference to temporary used 
# 401: base_class_with_nonvirtual_dtor
# 550: set_but_not_used
# 835: unreferenced_function_param
# WARNING_FLAGS   += -w0 -msg_display_number -msg_display_tag \
#                    -msg_disable 9,174,193,236,340,401,550,835

CCFLAGS         += $(CFLAGS) -ptr ptrepository $(WARNING_FLAGS)
DCFLAGS         += -g -O0
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -pthread -ltli -lrt -ptr ptrepository
OFLAGS          += -O4
PIC             =
ARFLAGS         = cruvZ
# When libraries are archived, a hash index is automatically created 
# so there is no need for ranlib
RANLIB          = /usr/bin/true
SOFLAGS         =  -shared -use_ld_input
SOBUILD         = /bin/rm -f $@; $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && \
		  ln $(VSHDIR)$*.o $@
