##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for the server-side ACE network services
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4 

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libnetsvcs.la

libnetsvcs_la_SOURCES = \
         Base_Optimizer.cpp  \
         Client_Logging_Handler.cpp  \
         Log_Message_Receiver.cpp  \
         Name_Handler.cpp  \
         Server_Logging_Handler.cpp  \
         Server_Logging_Handler_T.cpp  \
         TS_Clerk_Handler.cpp  \
         TS_Server_Handler.cpp  \
         Token_Handler.cpp

libnetsvcs_la_LDFLAGS = \
         -version-info @ACE_CURRENT@:@ACE_REVISION@:@ACE_AGE@

noinst_HEADERS = \
         Base_Optimizer.h  \
         Client_Logging_Handler.h  \
         Log_Message_Receiver.h  \
         Name_Handler.h  \
         Server_Logging_Handler.h  \
         Server_Logging_Handler_T.h  \
         TS_Clerk_Handler.h  \
         TS_Server_Handler.h  \
         Token_Handler.h

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations


## What do we about the below?  It was in the original libnetsvcs Makefile.
##  ifeq ($(SUPPRESS_DASH_G),1)
##  #### Build this target without -g on some platforms.
##  $(VDIR)Server_Logging_Handler.o $(VSHDIR)Server_Logging_Handler.$(SOEXT):
##          $(COMPILE-NO_DASH_G.cc) -o $@ $<
##  endif # SUPPRESS_DASH_G

