##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for ACE_RMCast library
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libACE_RMCast.la

libACE_RMCast_la_SOURCES = \
  RMCast.cpp \
  RMCast_Ack_Worker.cpp \
  RMCast_Fork.cpp \
  RMCast_Fragment.cpp \
  RMCast_IO_UDP.cpp \
  RMCast_Membership.cpp \
  RMCast_Module.cpp \
  RMCast_Module_Factory.cpp \
  RMCast_Partial_Message.cpp \
  RMCast_Proxy.cpp \
  RMCast_Reassembly.cpp \
  RMCast_Receiver_Module.cpp \
  RMCast_Reliable_Factory.cpp \
  RMCast_Reordering.cpp \
  RMCast_Resend_Handler.cpp \
  RMCast_Resend_Worker.cpp \
  RMCast_Retransmission.cpp \
  RMCast_Sequencer.cpp \
  RMCast_Singleton_Factory.cpp \
  RMCast_UDP_Event_Handler.cpp \
  RMCast_UDP_Proxy.cpp \
  RMCast_UDP_Reliable_Receiver.cpp \
  RMCast_UDP_Reliable_Sender.cpp

libACE_RMCast_la_LIBADD = $(top_builddir)/ace/libACE.la

## These are template source files.
TEMPLATE_FILES = \
  RMCast_Copy_On_Write.cpp \
  RMCast_Worker.cpp

HEADER_FILES = \
  RMCast.h \
  RMCast_Ack_Worker.h \
  RMCast_Copy_On_Write.h \
  RMCast_Export.h \
  RMCast_Fork.h \
  RMCast_Fragment.h \
  RMCast_IO_UDP.h \
  RMCast_Membership.h \
  RMCast_Module.h \
  RMCast_Module_Factory.h \
  RMCast_Partial_Message.h \
  RMCast_Proxy.h \
  RMCast_Reassembly.h \
  RMCast_Receiver_Module.h \
  RMCast_Reliable_Factory.h \
  RMCast_Reordering.h \
  RMCast_Resend_Handler.h \
  RMCast_Resend_Worker.h \
  RMCast_Retransmission.h \
  RMCast_Sequencer.h \
  RMCast_Singleton_Factory.h \
  RMCast_UDP_Event_Handler.h \
  RMCast_UDP_Proxy.h \
  RMCast_UDP_Reliable_Receiver.h \
  RMCast_UDP_Reliable_Sender.h \
  RMCast_Worker.h

INLINE_FILES = \
  RMCast.i \
  RMCast_Ack_Worker.i \
  RMCast_Copy_On_Write.i \
  RMCast_Fork.i \
  RMCast_Fragment.i \
  RMCast_IO_UDP.i \
  RMCast_Membership.i \
  RMCast_Module.i \
  RMCast_Module_Factory.i \
  RMCast_Partial_Message.i \
  RMCast_Proxy.i \
  RMCast_Reassembly.i \
  RMCast_Receiver_Module.i \
  RMCast_Reliable_Factory.i \
  RMCast_Reordering.i \
  RMCast_Resend_Handler.i \
  RMCast_Resend_Worker.i \
  RMCast_Retransmission.i \
  RMCast_Sequencer.i \
  RMCast_Singleton_Factory.i \
  RMCast_UDP_Event_Handler.i \
  RMCast_UDP_Proxy.i \
  RMCast_UDP_Reliable_Receiver.i \
  RMCast_UDP_Reliable_Sender.i \
  RMCast_Worker.i

pkginclude_HEADERS = \
  $(HEADER_FILES) \
  $(INLINE_FILES) \
  $(TEMPLATE_FILES)
