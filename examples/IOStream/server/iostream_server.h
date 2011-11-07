// $Id$

// This file defines the class needed for iostream_server.cpp - it's a separate
// file to satisfy IBM's xlC template instantiation.

#ifndef __IOSTREAM_SERVER_H
#define __IOSTREAM_SERVER_H

#include "ace/INET_Addr.h"
#include "ace/IOStream.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

#if !defined (ACE_LACKS_ACE_IOSTREAM)

// Declare a new type which will case an ACE_SOCK_Stream to behave
// like an iostream.  The new ACE_SOCK_IOStream type can be used
// anywhere an ACE_SOCK_Stream is used.

typedef ACE_IOStream<ACE_SOCK_Stream> ACE_SOCK_IOStream;

class Handler : public ACE_Svc_Handler<ACE_SOCK_IOStream, ACE_NULL_SYNCH>
  // = TITLE
  //     Extend the <ACE_Svc_Handler> template to do our bidding.
  //
  // = DESCRIPTION
  //     Create an <ACE_Svc_Handler> object based on our
  //     iostream/SOCK_Stream hybrid.  All of this is fairly standard
  //     until we get to the <handle_input> where we begin using the
  //     iostream characteristics of the peer.
{
public:
  // = Initialization and termination methods.
  Handler (void);
  ~Handler (void);

  // = <Svc_Handler> hooks.
  virtual int open (void *);

  // = <Event_Handler> hooks.
  virtual int handle_input (ACE_HANDLE);
};

#endif /* ACE_LACKS_ACE_IOSTREAM */

#endif /* __IOSTREAM_SERVER_H */
