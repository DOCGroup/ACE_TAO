##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.PSDL.am 

lib_LTLIBRARIES = libTAO_PSDL.la

libTAO_PSDL_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_PSDL_BUILD_DLL

libTAO_PSDL_la_SOURCES = \
  PSDL_Node.cpp \
  PSDL_Interpreter.cpp \
  PSDL_Visitor_Factory.cpp \
  PSDL_Node_Visitor.cpp \
  PSDL_Module_Visitor.cpp \
  PSDL_Type_Dcl_Visitor.cpp \
  PSDL_Struct_Visitor.cpp \
  PSDL_Enum_Type_Visitor.cpp \
  PSDL_Interface_Visitor.cpp \
  PSDL_Exception_Visitor.cpp \
  PSDL_Typecode_Defn.cpp \
  PSDL_Scope.cpp \
  PSDL_Root_Scope.cpp \
  PSDL_Simple_Scope.cpp \
  PSDL_Module_Scope.cpp \
  PSDL_Interface_Scope.cpp \
  PSDL_Struct_Scope.cpp \
  PSDL_Exception_Scope.cpp \
  PSDL_Op_Dcl_Scope.cpp \
  PSDL_Scope_Visitor.cpp \
  PSDL_Stream.cpp \
  PSDL_Extern.cpp \
  PSDL_l.cpp \
  PSDL_y.cpp

libTAO_PSDL_la_LDFLAGS = \
  -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_PSDL_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

noinst_HEADERS = \
  PSDL_Struct_Visitor.h \
  PSDL_Interface_Scope.h \
  PSDL_Typecode_Defn.h \
  PSDL_Type_Dcl_Visitor.h \
  PSDL_Interpreter.h \
  PSDL_Enum_Type_Visitor.h \
  PSDL_Struct_Scope.h \
  PSDL_Op_Dcl_Scope.h \
  PSDL_Module_Scope.h \
  PSDL_Visitor_Factory.h \
  PSDL_Node_Visitor.h \
  PSDL_Exception_Visitor.h \
  PSDL_Scope_Visitor.h \
  PSDL_Stream.h \
  PSDL_Root_Scope.h \
  PSDL_Exception_Scope.h \
  PSDL_Scope.h \
  PSDL_Extern.h \
  PSDL_Simple_Scope.h \
  PSDL_Module_Visitor.h \
  PSDL_Node.h \
  PSDL_y.h \
  PSDL_Interface_Visitor.h


## Makefile.PSDL_Datastore.am 

lib_LTLIBRARIES += libTAO_PSDL_Datastore.la

libTAO_PSDL_Datastore_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_PSDL_BUILD_DLL

libTAO_PSDL_Datastore_la_SOURCES = \
  PSDL_Code_Gen.cpp \
  PSDL_OctetSeq.cpp \
  PSDL_String.cpp \
  PSDL_Datastore.cpp

libTAO_PSDL_Datastore_la_LDFLAGS = \
  -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_PSDL_Datastore_la_LIBADD = \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

noinst_HEADERS += \
  PSDL_Datastore.h \
  PSDL_String.h \
  PSDL_OctetSeq.h \
  PSDL_Code_Gen.h


## Makefile.PSDL_Parser.am 

bin_PROGRAMS = psdl_tao

psdl_tao_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR)

psdl_tao_SOURCES = \
  Dump_Visitor.cpp \
  psdl_tao.cpp

psdl_tao_LDADD = \
  libTAO_PSDL.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

noinst_HEADERS += \
  Dump_Visitor.h

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
