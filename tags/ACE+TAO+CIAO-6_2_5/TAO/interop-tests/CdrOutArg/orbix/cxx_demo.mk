#$Id$
#################################################################################
#
#        Copyright (c) 1993-2009 Progress Software Corporation and/or
#                         its affiliates or subsidiaries.
#                             All Rights Reserved.
#
#################################################################################



ART_LIB_DIR=			$(IT_PRODUCT_DIR)/asp/$(IT_PRODUCT_VER)/lib
ART_CXX_INCLUDE_DIR=		$(IT_PRODUCT_DIR)/asp/$(IT_PRODUCT_VER)/include

IT_PRODUCT_SHLIB_DIR=		$(IT_PRODUCT_DIR)/shlib
IT_PRODUCT_DEF_SHLIB_DIR=     	$(IT_PRODUCT_DIR)/shlib/default


CXX=				g++

CXXFLAGS=			-I$(ART_CXX_INCLUDE_DIR)\
				-g3 -m32 -mtune=pentium3 -march=i586 -pipe -Wno-ctor-dtor-privacy   -D_REENTRANT\
				$(EXTRA_CXXFLAGS)\
				$(CXXLOCAL_DEFINES)

CXXLDFLAGS=			$(CXXFLAGS) -rdynamic -L/usr/local/lib -Wl,-t -lpthread -lrt


CLEAN_TEMPL_REPOSITORY=

CXXSHLIBFLAGS=			$(CXXFLAGS) -fPIC

SHLIB_CXX_COMPILER_ID=          gcc34

SHLIBSUFFIX=			so

DEMO_SHLIB_FILE_NAME=

SHLIBLDFLAGS=			-rdynamic -Wl,-Bdynamic -Wl,-export-dynamic -shared -rdynamic -L/usr/local/lib -Wl,-t -lpthread -lrt

LDLIBS=				-L$(ART_LIB_DIR)\
				-L$(IT_PRODUCT_SHLIB_DIR)\
				-L$(IT_PRODUCT_DEF_SHLIB_DIR) $(EXTRA_LIB_PATH)\
				$(LINK_WITH)

ORACLE_BIN_DIR=                 $(ORACLE_HOME)/bin
ORACLE_LIB_DIR=                 $(ORACLE_HOME)/lib
ORACLE_INCLUDES=                -I$(ORACLE_HOME)/precomp/public
PRO_CXX_FLAGS=                  -fpermissive
PROC_INCLUDES=                  include=/usr/include include=$(ART_CXX_INCLUDE_DIR)


OBJS=				$(SOURCES:.cxx=.o)

.SUFFIXES:			.pc .cxx

# These demo makefiles do not support clearmake parallel builds
# .NOTPARALLEL:

.pc.cxx:
	$(ORACLE_BIN_DIR)/proc $< code=cpp lines=yes parse=partial cpp_suffix=cxx threads=no define=IT_PROC $(PROC_INCLUDES)

.cxx.o:
	$(RM) $@
	$(CXX) -c $(CXXFLAGS) -o $@ $<

