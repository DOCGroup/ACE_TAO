#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libmpeg_client
LIB2     = $(LIBNAME).a
SHLIB2   = $(LIBNAME).$(SOEXT)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES= 24bit            \
       2x2              \
       ab               \
       audio            \
       decoders         \
       file             \
       fs2              \
       fs2fast          \
       fs4              \
       global           \
       gray             \
       hybrid           \
       hybriderr        \
       info             \
       jrevdct          \
       mb_ordered       \
       mono             \
       motionvector     \
       ordered          \
       ordered2         \
       para             \
       parseblock       \
       prog             \
       ui               \
       util             \
       util32           \
       vb               \
       vd               \
       video            \
       vp               \
       Command_Handler

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT) -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))

##LIBS 	+= -lTAO -lACE

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (AV,$(findstring AV,$(TAO_ORBSVCS)))
  LIB = $(LIB2)
  SHLIB = $(SHLIB2)
else  # ! AV
  override INSTALL =
endif # ! AV

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS +=  -L/project/doc/pkg/X11/lib/ -lXm -lXt -lXext -lSM -lICE -lX11 -L/project/doc/pkg/X11/lib -lsocket -lnsl -ldl
CPPFLAGS += -i -DSH_MEM -DNDEBUG  -I/project/doc/pkg/X11/include -L/project/doc/pkg/X11/lib

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
