#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the example programs in "C++ Network Programming,
#	Vol. 1, Resolving Complexity Using Patterns and ACE"
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

all :  Logging_Client \
       Iterative_Logging_Server \
       Process_Per_Connection_Logging_Server \
       Thread_Per_Connection_Logging_Server

Logging_Client :
	$(MAKE) -f Logging_Client.mak

Iterative_Logging_Server :
	$(MAKE) -f Iterative_Logging_Server.mak

Process_Per_Connection_Logging_Server :
	$(MAKE) -f Process_Per_Connection_Logging_Server.mak

RT_Thread_Per_Connection_Logging_Server :
	$(MAKE) -f RT_Thread_Per_Connection_Logging_Server.mak

Thread_Per_Connection_Logging_Server :
	$(MAKE) -f Thread_Per_Connection_Logging_Server.mak
