/* -*- C++ -*- */
// @(#)Channel_Connector.h	1.1	10/18/96


// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Channel_Connector.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CHANNEL_CONNECTOR)
#define _CHANNEL_CONNECTOR

#include "ace/Connector.h"
#include "Thr_Channel.h"

class Channel_Connector : public ACE_Connector<Channel, CHANNEL_PEER_CONNECTOR>
  // = TITLE
  //    A concrete factory class that setups connections to peerds 
  //    and produces a new Channel object to do the dirty work...
{
public:
  Channel_Connector (void);

  // Initiate (or reinitiate) a connection on the Channel.
  int initiate_connection (Channel *, 
			   ACE_Synch_Options & = ACE_Synch_Options::synch);

protected:  
  // Override the connection-failure method to add timer support.
  virtual int handle_close (ACE_HANDLE sd, ACE_Reactor_Mask);
};

#endif /* _CHANNEL_CONNECTOR */
