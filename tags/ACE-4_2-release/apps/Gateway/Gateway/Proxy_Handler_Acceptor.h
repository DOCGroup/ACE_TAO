/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Proxy_Handler_acceptor.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_PROXY_HANDLER_ACCEPTOR)
#define _PROXY_HANDLER_ACCEPTOR

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "Proxy_Handler.h"

// Forward declaration
class ACE_Event_Channel;

class Proxy_Handler_Acceptor : public ACE_Acceptor<Proxy_Handler_Factory, ACE_SOCK_ACCEPTOR>
  // = TITLE
  //    A concrete factory class that setups connections to peerds 
  //    and produces a new Proxy_Handler object to do the dirty work...
{
public:
  Proxy_Handler_Acceptor (ACE_Event_Channel &);

protected:  
  ACE_Event_Channel &event_channel_;
  // Reference to the event channel.
};

#endif /* _PROXY_HANDLER_ACCEPTOR */
