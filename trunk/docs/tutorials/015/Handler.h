// $Id$

#ifndef HANDLER_H
#define HANDLER_H

#include "ace/Svc_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "Protocol_Stream.h"

/* Just your basic event handler.  We use ACE_Svc_Handler<> as a
   baseclass so that it can maintain the peer() and other details for
   us.  We're not going to activate() this object, so we can get away
   with the NULL synch choice.  */
class Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  Handler (void);
  ~Handler (void);

  // Called by the acceptor when we're created in response to a client
  // connection.
  int open (void *);

  // Called when it's time for us to be deleted.  We take care of
  // removing ourselves from the reactor and shutting down the peer()
  // connectin.
  void destroy (void);

  // Called when it's time for us to go away.  There are subtle
  // differences between destroy() and close() so don't try to use
  // either for all cases.
  int close (u_long);

protected:
  // Respond to peer() activity.
  int handle_input (ACE_HANDLE);

  // This will be called when handle_input() returns a failure code.
  // That's our signal that it's time to begin the shutdown process.
  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask mask);
private:

  // Like the Client, we have to abide by the protocol requirements.
  // We use a local Protocol_Stream object to take care of those
  // details.  For us, I/O then just becomes a matter of interacting
  // with the stream.
  Protocol_Stream stream_;

  Protocol_Stream &stream (void)
  {
    return this->stream_;
  }
};

#endif /* HANDLER_H */
