#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for TAO
#----------------------------------------------------------------------------
shared_libs_only=1

MAKEFILE = Makefile
LIBNAME  = libTAO
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).so

# @@ Need to clean up this makefile...it's a MESS!!!! (cjc)
# These are components that are only headers and must be installed
CORBA_HDRS = orb sequence stub orbconf objtable optable $(CORBA_SRCS)
CORBA_SRCS = any corbacom except nvlist object orbobj principa \
	request svrrqst poa typecode

# These are components that are only headers and must be installed
OTHERS_HDRS = align cdr giop iiopobj iioporb xdr connect params marshal debug \
              default_client default_server server_factory client_factory orb_core
OTHERS_INLN = any poa cdr connect iiopobj iioporb object orbobj params svrrqst typecode marshal \
              default_client default_server server_factory client_factory orb_core
# These are components that are either header & source or source-only
OTHERS_SRCS = \
	giop \
	iiopobj \
	iioporb \
	invoke \
	params \
	default_client \
	default_server \
	server_factory \
	client_factory \
	orb_core \
	objtable \
	optable \
	connect \
	cdr \
	debug \
	interp \
	marshal \
	encode \
	decode \
	deep_copy \
	deep_free \
	tao_internals \
	tc_const

FILES	 = $(OTHERS_SRCS) $(CORBA_SRCS)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

COMPATDIR = $(TAO_ROOT)/tao/compat
COMPATFILES = objbase.h initguid.h

BIN = # t-xdr t-sizes

TEMPLATE_FILES = 

DEFS  	= $(addsuffix .h,$(OTHERS_HDRS))
LSRC	= $(addsuffix .cpp,$(FILES))
LSRC2	= $(addsuffix .cpp,$(TEMPLATE_FILES))

LDLIBS = -lACE
LIBS = -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) $(VLIB) $(VBIN)


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
#VSHDIR = obj/
#VSHOBJS = $(LSRC:%.cpp=$(VSHDIR)/$(notdir %).so)
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

#RLDFLAGS := $(subst -L,-Wl\,-rpath ,$(LDFLAGS))
#CPPFLAGS += -D__IIOP_BUILD -I$(COMPATDIR) -DDEBUG
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
CPPFLAGS += -I$(TAO_ROOT) -I$(COMPATDIR) $(TSS_ORB_FLAG)#-H
LDFLAGS += $(RLDFLAGS)
#DCFLAGS = -g
#OCFLAGS = -fast

INSINC = ../proto/include
INSLIB = ../proto/lib

# The first value is a nasty hack because $(VSHLIB) was not working
# for some reason :-(
INSTALL = $(INSLIB)/libTAO.$(SOEXT) \
	$(addprefix $(INSINC)/,$(addsuffix .h,$(OTHERS_HDRS)) $(addsuffix .i,$(OTHERS_INLN)) $(addsuffix .h,$(basename $(COMPATFILES)) ) ) \
	$(addprefix $(INSINC)/corba/, $(addsuffix .h,$(CORBA_SRCS) $(CORBA_HDRS) debug)) \


INST = install -c
INSTALL_BY_COPY = $(INST) $^ $@

$(INSINC)/%:	%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)
$(INSINC)/%:	$(COMPATDIR)/%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)
$(INSLIB)/%:	%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)
$(INSINC)/corba/%:	%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)

copy:	$(INSTALL)


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

