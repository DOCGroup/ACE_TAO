/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    IO_Handler_Connector.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_IO_HANDLER_CONNECTOR)
#define _IO_HANDLER_CONNECTOR

#include "ace/Connector.h"
#include "Thr_IO_Handler.h"

class IO_Handler_Connector : public ACE_Connector<IO_Handler, ACE_SOCK_CONNECTOR>
  // = TITLE
  //    A concrete factory class that setups connections to peerds 
  //    and produces a new IO_Handler object to do the dirty work...
{
public:
  IO_Handler_Connector (void);

  // Initiate (or reinitiate) a connection on the IO_Handler.
  int initiate_connection (IO_Handler *, 
			   ACE_Synch_Options & = ACE_Synch_Options::synch);

protected:  
  // Override the connection-failure method to add timer support.
  virtual int handle_close (ACE_HANDLE sd, ACE_Reactor_Mask);
};

#endif /* _IO_HANDLER_CONNECTOR */
