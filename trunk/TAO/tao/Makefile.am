##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for TAO
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

# Build a libtool library, libTAO.la for installation in libdir.
lib_LTLIBRARIES = libTAO.la
libTAO_la_SOURCES = \
         Active_Object_Map.cpp  \
         Any.cpp  \
         CDR.cpp  \
         Client_Strategy_Factory.cpp  \
         Connect.cpp  \
         CurrentC.cpp  \
         CurrentS.cpp  \
         DynAnyC.cpp  \
         DynAnyS.cpp  \
         DynAny_i.cpp  \
         DynArray_i.cpp  \
         DynEnum_i.cpp  \
         DynSequence_i.cpp  \
         DynStruct_i.cpp  \
         DynUnion_i.cpp  \
         Environment.cpp  \
         Exception.cpp  \
         Forwarding_Servant.cpp  \
         GIOP.cpp  \
         IIOP_Interpreter.cpp  \
         IIOP_ORB.cpp  \
         IIOP_Object.cpp  \
         InconsistentTypeCodeC.cpp  \
         Invocation.cpp  \
         Managed_Types.cpp  \
         Marshal.cpp  \
         NVList.cpp  \
         ORB.cpp  \
         ORB_Core.cpp  \
         Object.cpp  \
         Object_KeyC.cpp  \
         Operation_Table.cpp  \
         POA.cpp  \
         POAC.cpp  \
         POAS.cpp  \
         PolicyC.cpp  \
         PolicyS.cpp  \
         Principal.cpp  \
         Request.cpp  \
         Sequence.cpp  \
         Servant_Base.cpp  \
         Server_Request.cpp  \
         Server_Strategy_Factory.cpp  \
         Stub.cpp  \
         TAO.cpp  \
         TAO_Internal.cpp  \
         Typecode.cpp  \
         Typecode_Constants.cpp  \
         Union.cpp  \
         append.cpp  \
         debug.cpp  \
         decode.cpp  \
         deep_free.cpp  \
         default_client.cpp  \
         default_server.cpp  \
         encode.cpp  \
         params.cpp  \
         skip.cpp

## The following is an excerpt from the "libtool" manual, written by
## Gordon Matzigkeit:
##
## This flag accepts an argument of the form `current[:revision[:age]]'. So,
## passing `-version-info 3:12:1' sets current to 3, revision to 12, and age
## to 1. 
## If either revision or age are omitted, they default to 0. Also note that
## age must be less than or equal to the current interface number. 
##
## Here are a set of rules to help you update your library version
## information: 
##
##     1.Start with version information of `0:0:0' for each libtool library. 
##     2.Update the version information only immediately before a public
##       release of your software. More frequent updates are unnecessary, and
##       only guarantee that the current interface number gets larger faster. 
##     3.If the library source code has changed at all since the last update,
##       then increment revision (`c:r:a' becomes `c:r+1:a'). 
##     4.If any interfaces have been added, removed, or changed since the last
##       update, increment current, and set revision to 0. 
##     5.If any interfaces have been added since the last public release, then
##       increment age. 
##     6.If any interfaces have been removed since the last public release,
##       then set age to 0. 
##
## Never try to set the interface numbers so that they correspond to the
## release number of your package. This is an abuse that only fosters
## misunderstanding of the purpose of library versions. Instead, use the
## `-release' flag (see section 6.4 Managing release information), but be
## warned that every release of your package will not be binary compatibility
## with any other release. 

## The below "-version-info" isn't being used the way libtool intends
## it to be used but we do it that way to make the version numbers
## that TAO uses match those created by libtool from "-version-info."
##libTAO_la_LDFLAGS = -version-info 2:41:2   ## 2-2=0 --> libTAO.so.0.2.41
libTAO_la_LDFLAGS = -version-info @TAO_CURRENT@:@TAO_REVISION@:@TAO_AGE@

## These are source files that are needed by the header files if the
## ACE_TEMPLATES_REQUIRE_SOURCE or ACE_TEMPLATES_REQUIRE_PRAGMA
## macro is defined.
if TEMPLATES_REQUIRE_SOURCE
TEMPLATE_FILES = \
         DynUnion_i_T.cpp  \
         ORB_Strategies_T.cpp  \
         Sequence_T.cpp  \
         poa_T.cpp  \
         varout.cpp
else
TEMPLATE_FILES = 
endif

pkgincludedir = $(prefix)/include/tao
pkginclude_HEADERS = \
         Active_Object_Map.h  \
         Active_Object_Map.i  \
         Align.h  \
         Any.h  \
         Any.i  \
         CDR.h  \
         CDR.i  \
         Client_Strategy_Factory.h  \
         Connect.h  \
         Connect.i  \
         CurrentC.h  \
         CurrentC.i  \
         DynAnyC.h  \
         DynAnyC.i  \
         DynAny_i.h  \
         DynArray_i.h  \
         DynEnum_i.h  \
         DynSequence_i.h  \
         DynStruct_i.h  \
         DynUnion_i.h  \
         DynUnion_i_T.h  \
         Environment.h  \
         Environment.i  \
         Exception.h  \
         Exception.i  \
         Forwarding_Servant.h  \
         GIOP.h  \
         GIOP.i  \
         IIOP_Interpreter.h  \
         IIOP_ORB.h  \
         IIOP_ORB.i  \
         IIOP_Object.h  \
         IIOP_Object.i  \
         InconsistentTypeCodeC.h  \
         Invocation.h  \
         Invocation.i  \
         Managed_Types.h  \
         Managed_Types.i  \
         Marshal.h  \
         Marshal.i  \
         NVList.h  \
         NVList.i  \
         ORB.h  \
         ORB.i  \
         ORB_Core.h  \
         ORB_Core.i  \
         ORB_Strategies_T.h  \
         ORB_Strategies_T.i  \
         Object.h  \
         Object.i  \
         Object_KeyC.h  \
         Object_KeyC.i  \
         Object_Table.h  \
         Object_Table.i  \
         Operation_Table.h  \
         POA.h  \
         POA.i  \
         POAC.h  \
         POAC.i  \
         POAS.h  \
         POAS.i  \
         POA_CORBA.h  \
         PolicyC.h  \
         PolicyC.i  \
         Principal.h  \
         Principal.i  \
         Request.h  \
         Request.i  \
         Sequence.h  \
         Sequence.i  \
         Sequence_T.h  \
         Sequence_T.i  \
         Servant_Base.h  \
	 Servant_Base.i  \
         Server_Request.h  \
         Server_Request.i  \
         Server_Strategy_Factory.h  \
         Stub.h  \
         Stub.i  \
         TAO.h  \
         TAO_Internal.h  \
         TAO_Internal.i  \
         Timeprobe.h  \
         Typecode.h  \
         Typecode.i  \
         Union.h  \
         config.h  \
         corba.h  \
         debug.h  \
         default_client.h  \
         default_client.i  \
         default_server.h  \
         default_server.i  \
         orbconf.h  \
         params.h  \
         params.i  \
         poa_T.h  \
         poa_macros.h  \
         singletons.h  \
         try_macros.h  \
         varout.h  \
         varout.i  \
         $(TEMPLATE_FILES)
## Make sure $(TEMPLATE_FILES) is in the above header list!


## noinst_PROGRAMS = t-sizes

## t_sizes_SOURCES = t-sizes.cpp

EXTRA_DIST = \
         t-sizes.cpp
