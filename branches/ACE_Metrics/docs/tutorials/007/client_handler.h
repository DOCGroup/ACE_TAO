// $Id$

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

/* Our client handler must exist somewhere in the ACE_Event_Handler
   object hierarchy.  This is a requirement of the ACE_Reactor because
   it maintains ACE_Event_Handler pointers for each registered event
   handler.  You could derive our Client_Handler directly from
   ACE_Event_Handler but you still have to have an ACE_SOCK_Stream for
   the actually connection.  With a direct derivative of
   ACE_Event_Handler, you'll have to contain and maintain an
   ACE_SOCK_Stream instance yourself.  With ACE_Svc_Handler (which is
   a derivative of ACE_Event_Handler) some of those details are
   handled for you.  */

#include "ace/Svc_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"

class Client_Acceptor;
class Thread_Pool;

/* Another feature of ACE_Svc_Handler is it's ability to present the
   ACE_Task<> interface as well.  That's what the ACE_NULL_SYNCH
   parameter below is all about.  That's beyond our scope here but
   we'll come back to it in the next tutorial when we start looking at
   concurrency options.  */
class Client_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;

  // Constructor...
  Client_Handler (void);

  /* The destroy() method is our preferred method of destruction.  We
    could have overloaded the delete operator but that is neither easy
    nor intuitive (at least to me).  Instead, we provide a new method
    of destruction and we make our destructor protected so that only
    ourselves, our derivatives and our friends can delete us. It's a
    nice compromise.  */
  void destroy (void);

  /* Most ACE objects have an open() method.  That's how you make them
    ready to do work.  ACE_Event_Handler has a virtual open() method
    which allows us to create this overrride.  ACE_Acceptor<> will
    invoke this method after creating a new Client_Handler when a
    client connects. Notice that the parameter to open() is a void*.
    It just so happens that the pointer points to the acceptor which
    created us.  You would like for the parameter to be an
    ACE_Acceptor<>* but since ACE_Event_Handler is generic, that would
    tie it too closely to the ACE_Acceptor<> set of objects.  In our
    definition of open() you'll see how we get around that.  */
  int open (void *acceptor);

  /* When an ACE_Task<> object falls out of the svc() method, the
    framework will call the close() method.  That's where we want to
    cleanup ourselves if we're running in either thread-per-connection
    or thread-pool mode.  */
  int close (u_long flags = 0);

  /* When there is activity on a registered handler, the
    handle_input() method of the handler will be invoked.  If that
    method returns an error code (eg -- -1) then the reactor will
    invoke handle_close() to allow the object to clean itself
    up. Since an event handler can be registered for more than one
    type of callback, the callback mask is provided to inform
    handle_close() exactly which method failed.  That way, you don't
    have to maintain state information between your handle_* method
    calls. The <handle> parameter is explained below...  As a
    side-effect, the reactor will also invoke remove_handler() for the
    object on the mask that caused the -1 return.  This means that we
    don't have to do that ourselves!  */
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask mask);

  /* When we register with the reactor, we're going to tell it that we
    want to be notified of READ events.  When the reactor sees that
    there is read activity for us, our handle_input() will be
    invoked. The _handleg provided is the handle (file descriptor in
    Unix) of the actual connection causing the activity.  Since we're
    derived from ACE_Svc_Handler<> and it maintains it's own peer
    (ACE_SOCK_Stream) object, this is redundant for us.  However, if
    we had been derived directly from ACE_Event_Handler, we may have
    chosen not to contain the peer.  In that case, the <handle> would
    be important to us for reading the client's data.  */
  int handle_input (ACE_HANDLE handle);

protected:

  /* If the Client_Acceptor which created us has chosen a
    thread-per-connection strategy then our open() method will
    activate us into a dedicate thread.  The svc() method will then
    execute in that thread performing some of the functions we used to
    leave up to the reactor.  */
  int svc (void);

  /* This has nothing at all to do with ACE.  I've added this here as
    a worker function which I will call from handle_input().  That
    allows me to introduce concurrencly in later tutorials with a no
    changes to the worker function.  You can think of process() as
    application-level code and everything elase as
    application-framework code.  */
  int process (char *rdbuf, int rdbuf_len);

  /* We don't really do anything in our destructor but we've declared
    it to be protected to prevent casual deletion of this object.  As
    I said above, I really would prefer that everyone goes through the
    destroy() method to get rid of us.  */
  ~Client_Handler (void);

  /* When we get to the definition of Client_Handler we'll see that
    there are several places where we go back to the Client_Acceptor
    for information.  It is generally a good idea to do that through
    an accesor rather than using the member variable directly.  */
  Client_Acceptor *client_acceptor (void)
  {
    return this->client_acceptor_;
  }

  /* And since you shouldn't access a member variable directly,
    neither should you set (mutate) it.  Although it might seem silly
    to do it this way, you'll thank yourself for it later.  */
  void client_acceptor (Client_Acceptor *_client_acceptor)
  {
    this->client_acceptor_ = _client_acceptor;
  }

  /* The concurrency() accessor tells us the current concurrency
    strategy.  It actually queries the Client_Acceptor for it but by
    having the accessor in place, we could change our implementation
    without affecting everything that needs to know.  */
  int concurrency (void);

  /* Likewise for access to the Thread_Pool that we belong to.  */
  Thread_Pool * thread_pool (void);

  Client_Acceptor *client_acceptor_;

  /* For some reason I didn't create accessor/mutator methods for
    this.  So much for consistency....

    This variable is used to remember the thread in which we were
    created: the "creator" thread in other words.  handle_input()
    needs to know if it is operating in the main reactor thread (which
    is the one that created us) or if it is operating in one of the
    thread pool threads.  More on this when we get to handle_input().  */
  ACE_thread_t creator_;
};

#endif /* CLIENT_HANDLER_H */
