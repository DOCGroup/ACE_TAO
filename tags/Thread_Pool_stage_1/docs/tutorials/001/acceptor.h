// $Id$

#ifndef _CLIENT_ACCEPTOR_H
#define _CLIENT_ACCEPTOR_H

/* A SOCK_Acceptor knows how to accept socket connections.  We'll use
  one of those at the heart of our Logging_Acceptor.  */
#include "ace/SOCK_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* An Event_Handler is what you register with ACE_Reactor.  When
  events occur, the reactor will callback on the Event_Handler.  More
  on that in a few lines.  */
#include "ace/Event_Handler.h"

/* When a client connects, we'll create a Logging_Handler to deal with
  the connection.  Here, we bring in that declaration.  */
#include "logger.h"

/* Our Logging_Acceptor is derived from ACE_Event_Handler.  That lets
  the reactor treat our acceptor just like every other handler.  */
class Logging_Acceptor : public ACE_Event_Handler
{
public:

  /* For this simple case we won't bother with either constructor or
    destructor.  In a real application you would certainly have them.  */

  /* Here's the open() method we called from main().  We have two
    things to accomplish here: (1) Open the acceptor so that we can
    hear client requests and (2) register ourselves with the reactor
    so that we can respond to those requests.  */
  int open (const ACE_INET_Addr &addr,
            ACE_Reactor *reactor)
  {
    /* Perform the open() on the acceptor.  We pass through the
      address at which main() wants us to listen.  The second
      parameter tells the acceptor it is OK to reuse the address.
      This is necessary sometimes to get around closed connections
      that haven't timed out.  */
    if (this->peer_acceptor_.open (addr, 1) == -1)
      return -1;

    /* Remember the reactor we're using.  We'll need it later when we
      create a client connection handler.  */
    reactor_ = reactor;

    /* Now we can register with the reactor we were given.  Since the
      reactor pointer is global, we could have just used that but it's
      gross enough already.  Notice that we can pass 'this' right into
      the registration since we're derived from ACE_Event_Handler.  We
      also provide ACCEPT_MASK to tell the reactor that we want to
      know about accept requests from clients.  */
    return reactor->register_handler (this,
                                      ACE_Event_Handler::ACCEPT_MASK);
  }

private:

  /* To provide multi-OS abstraction, ACE uses the concept of
    "handles" for connection endpoints.  In Unix, this is a
    traditional file descriptor (or integer).  On other OS's, it may
    be something else.  The reactor will need to get the handle (file
    descriptor) to satisfy it's own internal needs.  Our relevant
    handle is the handle of the acceptor object, so that's what we
    provide.  */
  ACE_HANDLE get_handle (void) const
  {
    return this->peer_acceptor_.get_handle ();
  }

  /* When an accept request arrives, the reactor will invoke the
    handle_input() callback.  This is where we deal with the
    connection request.  */
  virtual int handle_input (ACE_HANDLE handle)
  {
    /* The handle provided to us by the reactor is the one that
      triggered our up-call.  In some advanced situations, you might
      actually register a single handler for multiple connections.
      The _handle parameter is a way to sort 'em out.  Since we don't
      use that here, we simply ignore the parameter with the
      ACE_UNUSED_ARG() macro.  */
    ACE_UNUSED_ARG (handle);

    Logging_Handler *svc_handler;

    /* In response to the connection request, we create a new
      Logging_Handler.  This new object will be used to interact with
      the client until it disconnects.  Note how we use the
      ACE_NEW_RETURN macro, which returns -1 if operator new fails. */
    ACE_NEW_RETURN (svc_handler,
                    Logging_Handler,
                    -1);

    /* To complete the connection, we invoke the accept() method call
      on the acceptor object and provide it with the connection
      handler instance.  This transfers "ownership" of the connection
      from the acceptor to the connection handler.  */
    if (this->peer_acceptor_.accept (*svc_handler) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p",
                         "accept failed"),
                        -1);

    /* Again, most objects need to be open()ed before they are useful.
      We'll give the handler our reactor pointer so that it can
      register for events as well.  If the open fails, we'll force a
      close().  */
    if (svc_handler->open (reactor_) == -1)
      svc_handler->close ();

    return 0;
  }

protected:

  /* Our acceptor object instance */
  ACE_SOCK_Acceptor peer_acceptor_;

  /* A place to remember our reactor pointer */
  ACE_Reactor *reactor_;
};

#endif /* _CLIENT_ACCEPTOR_H */

