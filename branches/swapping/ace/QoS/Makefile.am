##  Process this file with automake to create Makefile.in

includedir = @includedir@/ace/QoS

nobase_include_HEADERS =
lib_LTLIBRARIES =
## Makefile.QoS.am 

if BUILD_QOS
lib_LTLIBRARIES += libACE_QoS.la

libACE_QoS_la_CPPFLAGS = \
  -I$(top_builddir) \
  -I$(top_srcdir) \
  -I$(top_srcdir) \
  -DACE_QoS_BUILD_DLL

libACE_QoS_la_SOURCES = \
  QoS_Decorator.cpp \
  QoS_Manager.cpp \
  QoS_Session_Factory.cpp \
  QoS_Session_Impl.cpp \
  SOCK_Dgram_Mcast_QoS.cpp

libACE_QoS_la_LIBADD = \
 $(top_srcdir)/ace/libACE.la

nobase_include_HEADERS += \
  ACE_QoS_Export.h \
  QoS_Decorator.h \
  QoS_Manager.h \
  QoS_Session.h \
  QoS_Session_Factory.h \
  QoS_Session_Impl.h \
  SOCK_Dgram_Mcast_QoS.h \
  QoS_Session_Impl.i \
  SOCK_Dgram_Mcast_QoS.i

endif BUILD_QOS

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB
