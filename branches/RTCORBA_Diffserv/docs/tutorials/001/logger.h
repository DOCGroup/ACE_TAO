// $Id$

#ifndef _CLIENT_HANDLER_H
#define _CLIENT_HANDLER_H

/* A connection handler will also be derived from ACE_Event_Handler so
  that we can register with a reactor.  */

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

/* Since we're doing TCP/IP, we'll need a SOCK_Stream for the
  connection.  */
#include "ace/SOCK_Stream.h"

class Logging_Handler : public ACE_Event_Handler
{
public:
  /*
    Like the acceptor, we're simple enough to avoid constructor and destructor.
    */

  /* To open the client handler, we have to register ourselves with
    the reactor.  Notice that we don't have to "open" our
    ACE_SOCK_Stream member variable.  Why?  Because the call to the
    acceptor's accept() method took care of those details for us.  */

  int open (ACE_Reactor *reactor)
  {
    /*
      Remember our reactor...
      */
    reactor_ = reactor;

    /* In this case we're using the READ_MASK.  Like the acceptor,
      handle_input() will be called due to this mask but it's a nice
      piece of bookkeeping to have separate masks for the separate
      types of activity.  */

    if (reactor->register_handler (this,
                                   ACE_Event_Handler::READ_MASK) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) can't register with reactor\n"),
                        -1);
    return 0;
  }

  /* If we're explicitly closed we'll close our "file handle".  The
    net result is to close the connection to the client and remove
    ourselves from the reactor if we're registered */

  int close (void)
  {
    return this->handle_close (ACE_INVALID_HANDLE,
                               ACE_Event_Handler::RWE_MASK);
  }

  /* This is a bit of magic...  When we call the accept() method of
    the acceptor object, it wants to do work on an ACE_SOCK_Stream.
    We have one of those as our connection to the client but it would
    be gross to provide a method to access that object.  It's much
    cooler if the acceptor can just treat the Logging_Handler as an
    ACE_SOCK_Stream.  Providing this cast operator lets that happen
    cleanly.  */

  operator ACE_SOCK_Stream &()
  {
    return this->cli_stream_;
  }

protected:

  /* Again, like the acceptor, we need to provide the connection
    handle to the reactor.  */

  ACE_HANDLE get_handle (void) const
  {
    return this->cli_stream_.get_handle ();
  }

  /* And here's the handle_input().  This is really the workhorse of
    the application.  */

  virtual int handle_input (ACE_HANDLE)
  {
    /*
      Create and initialize a small receive buffer.  The extra byte is
      there to allow us to have a null-terminated string when it's over.
     */
    char buf[BUFSIZ + 1];

    /* Invoke the recv() method of the ACE_SOCK_Stream to get some
      data.  It will return -1 if there is an error.  Otherwise, it
      will return the number of bytes read.  Of course, if it read
      zero bytes then the connection must be gone.  How do I know
      that?  Because handle_input() would not be called by the reactor
      if there wasn't *some* kind of activity and a closed connection
      looks like a read request to the reactor.  But when you read
      from a closed connection you'll read zero bytes.

      Notice that in the error case or closed case we return -1.  That
      tells the reactor to call our handle_close() where we'll take
      care of shutting down cleanly.

      Although we don't make use of them, there are additional
      parameters you can use with the recv() call.  One of these is an
      ACE_Time_Value that allows you to limit the amount of time
      blocking on the recv().  You would use that if you weren't sure
      if data was available.  Since we only get to handle_input() when
      data is ready, that would be redundant.  On the other hand, if
      you use recv_n() to read *exactly* a number of bytes then
      limiting the time you wait for those bytes might be good.  The
      other paramter that may come in handy is an integer
      <i>flags</i>.  This is passed directly to the underlying OS
      recv() call.  See the man page recv(2) and the header
      sys/socket.h for the gory details. */

    ssize_t retval;
    switch (retval = this->cli_stream_.recv (buf, BUFSIZ))
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
      buf[retval] = '\0';
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) from client: %s",
                  buf));
    }

    return 0;
  }

  /* When handle_input() returns -1, we'll end up here.  There are a
    few housekeeping chores to handle.  */

  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask _mask)
  {
    /* Remove ourselves from the reactor.  We have to include the
      DONT_CALL in the mask so that it won't call handle_close() on us
      again!  */
    reactor_->remove_handler (this,
                              _mask | ACE_Event_Handler::DONT_CALL);

    /* Close the socket that we're connected to the client with.  */
    cli_stream_.close ();

    /* Since we know we were dynamically allocated by the acceptor,
      now is a good time to get rid of ourselves.  */
    delete this;

    return 0;
  }

protected:

  /* Our peer connection.  */
  ACE_SOCK_Stream cli_stream_;

  /* Our reactor (and our acceptor's reactor).  */
  ACE_Reactor *reactor_;
};

#endif /* _CLIENT_HANDLER_H */

