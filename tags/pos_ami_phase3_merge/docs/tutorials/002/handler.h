// $Id$

#ifndef LOGGING_HANDLER_H
#define LOGGING_HANDLER_H

#include "ace/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "ace/Reactor.h"

/* Since we used the template to create the acceptor, we don't know if
  there is a way to get to the reactor it uses.  We'll take the easy
  way out and grab the global pointer.  (There is a way to get back to
  the acceptor's reactor that we'll see later on.)  */
extern ACE_Reactor *g_reactor;

/* This time we're deriving from ACE_Svc_Handler instead of
  ACE_Event_Handler.  The big reason for this is because it already
  knows how to contain a SOCK_Stream and provides all of the method
  calls needed by the reactor.  The second template parameter is for
  some advanced stuff we'll do with later servers.  For now, just use
  it as is...  */
class Logging_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:

  /* The Acceptor<> template will open() us when there is a new client
    connection.  */
  virtual int open (void *)
  {
    ACE_INET_Addr addr;

    /* Ask the peer() (held in our baseclass) to tell us the address
      of the cient which has connected.  There may be valid reasons
      for this to fail where we wouldn't want to drop the connection
      but I can't think of one. */
    if (this->peer ().get_remote_addr (addr) == -1)
      return -1;

    /* The Acceptor<> won't register us with it's reactor, so we have
      to do so ourselves.  This is where we have to grab that global
      pointer.  Notice that we again use the READ_MASK so that
      handle_input() will be called when the client does something.  */
    if (g_reactor->register_handler (this,
                                     ACE_Event_Handler::READ_MASK) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) can't register with reactor\n"),
                        -1);

    /* Here's another new treat.  We schedule a timer event.  This
      particular one will fire in two seconds and then every three
      seconds after that.  It doesn't serve any useful purpose in our
      application other than to show you how it is done.  */
    else if (g_reactor->schedule_timer (this,
                                        0,
                                        ACE_Time_Value (2),
                                        ACE_Time_Value (3)) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "can'(%P|%t) t register with reactor\n"),
                        -1);

    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connected with %s\n",
                addr.get_host_name ()));

    return 0;
  }

  /* This is a matter of style & maybe taste.  Instead of putting all
    of this stuff into a destructor, we put it here and request that
    everyone call destroy() instead of 'delete'.  */
  virtual void destroy (void)
  {
    /* Remove ourselves from the reactor */
    g_reactor->remove_handler 
      (this,
       ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

    /* Cancel that timer we scheduled in open() */
    g_reactor->cancel_timer (this);

    /* Shut down the connection to the client.  */
    this->peer ().close ();

    /* Free our memory.  */
    delete this;
  }

  /* If somebody doesn't like us, they will close() us.  Actually, if
    our open() method returns -1, the Acceptor<> will invoke close()
    on us for cleanup.  */
  virtual int close (u_long flags = 0)
  {
    /* The ACE_Svc_Handler baseclass requires the <flags> parameter.
      We don't use it here though, so we mark it as UNUSED.  You can
      accomplish the same thing with a signature like handle_input's
      below.  */
    ACE_UNUSED_ARG (flags);

    /*
      Clean up and go away.
      */
    this->destroy ();
    return 0;
  }

protected:

  /* Respond to input just like Tutorial 1.  */
  virtual int handle_input (ACE_HANDLE)
  {
    char buf[128];
    ACE_OS::memset (buf, 0, sizeof (buf));

    switch (this->peer ().recv (buf,
                                sizeof buf))
      {
      case -1:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %p bad read\n",
                           "client logger"),
                          -1);
      case 0:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) closing log daemon (fd = %d)\n",
                           this->get_handle ()),
                          -1);
      default:
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) from client: %s",
                    buf));
      }

    return 0;
  }

  /* When the timer expires, handle_timeout() will be called.  The
    'arg' is the value passed after 'this' in the schedule_timer()
    call.  You can pass in anything there that you can cast to a
    void*.  */
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) handling timeout from this = %u\n",
                this));
    return 0;
  }

  /*
    Clean ourselves up when handle_input() (or handle_timer()) returns -1
    */
  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask)
  {
    this->destroy ();
    return 0;
  }
};

#endif /* LOGGING_HANDLER_H */
