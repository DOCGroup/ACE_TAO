/* -*- C++ -*- */
// @(#)Logging_Acceptor.h	1.1	10/18/96


// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    Logging_Acceptor.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CLIENT_ACCEPTOR_H)
#define _CLIENT_ACCEPTOR_H

#include "ace/SOCK_Acceptor.h"
#include "ace/Event_Handler.h"

class Logging_Acceptor : public ACE_Event_Handler
  // = TITLE
  //     Handle connection requests from remote client clients. 
  //
  // = DESCRIPTION
  //     Accepts client connection requests, creates Logging_Handler's
  //     to process them, and registers these Handlers with the
  //     ACE_Reactor Singleton.
{
friend class Logging_Handler;
public:
  Logging_Acceptor (void);
  ~Logging_Acceptor (void);

  int open (const ACE_INET_Addr &a);

private:
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

  ACE_SOCK_Acceptor peer_acceptor_;     
  // Passive connection acceptor factory.
};

#endif /* _CLIENT_ACCEPTOR_H */
