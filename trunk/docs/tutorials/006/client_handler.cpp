// $Id$

/* In client_handler.h I alluded to the fact that we'll mess around
   with a Client_Acceptor pointer.  To do so, we need the
   Client_Acceptor object declaration.

   We know that including client_handler.h is redundant because
   client_acceptor.h includes it.  Still, the sentry prevents
   double-inclusion from causing problems and it's sometimes good to
   be explicit about what we're using.

   On the other hand, we don't directly include any ACE header files
   here.  */
#include "client_acceptor.h"
#include "client_handler.h"

/* Our constructor doesn't do anything.  That's generally a good idea.
   Unless you want to start throwing exceptions, there isn't a really
   good way to indicate that a constructor has failed.  If I had my
   way, I'd have a boolean return code from it that would cause new to
   return 0 if I failed.  Oh well...  */
Client_Handler::Client_Handler (void)
{
}

/* Our destructor doesn't do anything either.  That is also by design.
   Remember, we really want folks to use destroy() to get rid of us.
   If that's so, then there's nothing left to do when the destructor
   gets invoked.  */
Client_Handler::~Client_Handler (void)
{
}

/* The much talked about destroy() method!  The reason I keep going on
   about this is because it's just a Bad Idea (TM) to do real work
   inside of a destructor.  Although this method is void, it really
   should return int so that it can tell the caller there was a
   problem.  Even as void you could at least throw an exception which
   you would never want to do in a destructor.  */
void 
Client_Handler::destroy (void)
{
  /* Tell the reactor to forget all about us.  Notice that we use the
    same args here that we use in the open() method to register
    ourselves.  In addition, we use the DONT_CALL flag to prevent
    handle_close() being called.  Since we likely got here due to
    handle_close(), that could cause a bit of nasty recursion!  */
  this->reactor ()->remove_handler (this,
                                    ACE_Event_Handler::READ_MASK 
                                    | ACE_Event_Handler::DONT_CALL);

  /* This is how we're able to tell folks not to use delete.  By
    deleting our own instance, we take care of memory leaks after
    ensuring that the object is shut down correctly.  */
  delete this;
}

/* As mentioned before, the open() method is called by the
   Client_Acceptor when a new client connection has been accepted.
   The Client_Acceptor instance pointer is cast to a void* and given
   to us here.  We'll use that to avoid some global data...  */
int 
Client_Handler::open (void *void_acceptor)
{
  /* We need this to store the address of the client that we are now
     connected to.  We'll use it later to display a debug message.  */
  ACE_INET_Addr addr;

  /* Our ACE_Svc_Handler baseclass gives us the peer() method as a way
    to access our underlying ACE_SOCK_Stream.  On that object, we can
    invoke the get_remote_addr() method to get get an ACE_INET_Addr
    having our client's address information. As with most ACE methods,
    we'll get back (and return) a -1 if there was any kind of error.
    Once we have the ACE_INET_Addr, we can query it to find out the
    client's host name, TCP/IP address, TCP/IP port value and so
    forth.  One word of warning: the get_host_name() method of
    ACE_INET_Addr may return you an empty string if your name server
    can't resolve it.  On the other hand, get_host_addr() will always
    give you the dotted-decimal string representing the TCP/IP
    address.  */
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  /* Convert the void* to a Client_Acceptor*.  You should probably use
    those fancy ACE_*_cast macros but I can never remember how/when to
    do so.  Since you can cast just about anything around a void*
    without compiler warnings be very sure of what you're doing when
    you do this kind of thing.  That's where the new-style cast
    operators can save you.  */
  Client_Acceptor *acceptor = (Client_Acceptor *) void_acceptor;

  /* Our Client_Acceptor is constructed with a concurrency strategy.
    Here, we go back to it to find out what that strategy was.  If
    thread-per-connection was selected then we simply activate a
    thread for ourselves and exit.  Our svc() method will then begin
    executing in that thread.

    If we are told to use the single-threaded strategy, there is no
    difference between this and the Tutorial 5 implementation.

    Note that if we're in thread-per-connection mode, open() is exited
    at this point.  Furthermore, thread-per-connection mode does not
    use the reactor which means that handle_input() and it's fellows
    are not invoked.  */
  if (acceptor->thread_per_connection ())
    return this->activate ();

  // ************************************************************************
  // From here on, we're doing the traditional reactor thing.  If
  // you're operating in thread-per-connection mode, this code does
  // not apply.
  // ************************************************************************

  /* Our reactor reference will be set when we register ourselves but
    I decided to go ahead and set it here.  No good reason really...  */
  this->reactor (acceptor->reactor ());

  /* If we managed to get the client's address then we're connected to
    a real and valid client.  I suppose that in some cases, the client
    may connect and disconnect so quickly that it is invalid by the
    time we get here. In any case, the test above should always be
    done to ensure that the connection is worth keeping.

    Now, regiser ourselves with a reactor and tell that reactor that
    we want to be notified when there is something to read.  Remember,
    we took our reactor value from the acceptor which created us in
    the first place.  Since we're exploring a single-threaded
    implementation, this is the correct thing to do.  */
  if (this->reactor ()->register_handler (this,
                                          ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) can't register with reactor\n"),
                      -1);

  /* Here, we use the ACE_INET_Addr object to print a message with the
    name of the client we're connected to.  Again, it is possible that
    you'll get an empty string for the host name if your DNS isn't
    configured correctly or if there is some other reason that a
    TCP/IP addreess cannot be converted into a host name.  */
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) connected with %s\n", addr.get_host_name ()));

  /* Always return zero on success.  */
  return 0;
}

/* As mentioned in the header, the typical way to close an object in a
   threaded context is to invoke it's close() method.  Since we
   already have a handle_close() method built to cleanup after us,
   we'll just forward the request on to that object.  */
int 
Client_Handler::close(u_long flags)
{
  ACE_UNUSED_ARG (flags);

  /* We use the destroy() method to clean up after ourselves.  That
    will take care of removing us from the reactor and then freeing
    our memory.  */
  this->destroy ();

  /* Don't forward the close() to the baseclass!  handle_close() above
    has already taken care of delete'ing.  Forwarding close() would
    cause that to happen again and things would get really ugly at
    that point!  */
  return 0;
}

/* In the open() method, we registered with the reactor and requested
   to be notified when there is data to be read.  When the reactor
   sees that activity it will invoke this handle_input() method on us.
   As I mentioned, the _handle parameter isn't useful to us but it
   narrows the list of methods the reactor has to worry about and the
   list of possible virtual functions we would have to override.

   Again, this is not used if we're in thread-per-connection mode.  */
int 
Client_Handler::handle_input (ACE_HANDLE handle)
{
  /* Some compilers don't like it when you fail to use a parameter.
    This macro will keep 'em quiet for you.  */
  ACE_UNUSED_ARG (handle);

  /* Now, we create and initialize a buffer for receiving the data.
    Since this is just a simple test app, we'll use a small buffer
    size.  */
  char buf[BUFSIZ];

  /* Invoke the process() method with a pointer to our data area.
    We'll let that method worry about interfacing with the data.  You
    might choose to go ahead and read the data and then pass the
    result to process().  However, application logic may require that
    you read a few bytes to determine what else to read...  It's best
    if we push that all into the application-logic level.  */
  return this->process (buf, sizeof (buf));
}

/* If we return -1 out of handle_input() or if the reactor sees other
   problems with us then handle_close() will be called.  The reactor
   framework will take care of removing us (due to the -1), so we
   don't need to use the destroy() method.  Instead, we just delete
   ourselves directly.  */
int 
Client_Handler::handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask mask)
{
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (mask);

  this->destroy ();
  return 0;
}

/* The ACE_Svc_Handler<> is ultimately derived from ACE_Task<>.  If
   you want to create a multi-threaded application, these are your
   tools!  Simply override the svc() method in your derivative and
   arrange for your activate() method to be called.  The svc() method
   then executes in the new thread.

   Of course, this is only valid if we're in thread-per-connection
   mode.  If we're using the reactor model, then svc() never comes
   into play.  */
int 
Client_Handler::svc(void)
{
  /* Like handle_input(), we create a buffer for loading the data.
    Doing so in handle_input() doesn't help any but there is a small
    performance increase by doing this here: the buffer is created
    once when the thread is created instead of for each invocation of
    process().  */
  char buf[BUFSIZ];

  // Forever...
  while( 1 )
    {
      /* Invoke the process() method to read and process the data.
        This is exactly the way it is used by handle_input().  That's
        the reason I created process() in the first place: so that it
        can be used in either concurrency strategy.  Since process()
        has all of our application-level logic, it's nice that it
        doesn't have to change when we decide to go multi-threaded.

        Notice that since the recv() method call in process() blocks until
        there is data ready, this thread doesn't consume any CPU time until
        there is actually data sent from the client.  */
      if (this->process(buf, sizeof (buf)) == -1)
        return -1;
    }

  return 0;
}

/* And, at last, we get to the application-logic level.  Out of
   everything we've done so far, this is the only thing that really
   has anything to do with what your application will do.  In this
   method we will read and process the client's data.  In a real
   appliation, you will probably have a bit more in main() to deal
   with command line options but after that point, all of the action
   takes place here.  */
int 
Client_Handler::process (char *rdbuf,
                         int rdbuf_len)
{
  /* Using the buffer provided for us, we read the data from the
    client. If there is a read error (eg -- recv() returns -1) then
    it's a pretty good bet that the connection is gone.  Likewise, if
    we read zero bytes then something wrong has happened.  The reactor
    wouldn't have called us if there wasn't some kind of read activity
    but there wouldn't be activity if there were no bytes to read...

    On the other hand, if we got some data then we can display it in a
    debug message for everyone to see.  */
  switch (this->peer ().recv (rdbuf, rdbuf_len))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, 
                         "(%P|%t) %p bad read\n",
                         "client"),
                        -1);
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) closing daemon (fd = %d)\n",
                         this->get_handle ()),
                        -1);
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) from client: %s",
                  rdbuf));
    }

  return 0;
}
