// $Id$

/* We need the connector object & we also bring in a simple string
  class.  */
#include "ace/SOCK_Connector.h"
#include "ace/SString.h"

/* In this tutorial, we extend SOCK_Stream by adding a few wrappers
  around the send_n() method.  */
class Client : public ACE_SOCK_Stream
{
public:
  // Basic constructor
  Client (void);

  /* Construct and open() in one call.  This isn't generally a good
    idea because you don't have a clean way to inform the caller when
    open() fails.  (Unless you use C++ exceptions.)  */
  Client (const char *server,
          u_short port);

  /* Open the connection to the server.  Notice that this mirrors the
    use of ACE_SOCK_Connector.  By providing our own open(), we can
    hide the connector from our caller & make it's interaction easier.  */
  int open (const char *server,
            u_short port);

  /* These are necessary if you're going to use the constructor that
    invokes open().  */
  int initialized (void) { return initialized_; }
  int error (void) { return error_; }

  /* This is where the coolness lies.  Most C++ folks are familiar
    with "cout << some-data."  It's a very handy and easy way to toss
    data around.  By adding these method calls, we're able to do the
    same thing with a socket connection.  */
  Client &operator<< (ACE_SString &str);
  Client &operator<< (char *str);
  Client &operator<< (int n);

protected:
  u_char initialized_;
  u_char error_;
};

/* The basic constructor just sets our flags to reasonable values.  */
Client::Client(void)
{
  initialized_ = 0;
  error_ = 0;
}

/* This constructor also sets the flags but then calls open().  If the
  open() fails, the flags will be set appropriately.  Use the two
  inline method calls initialized() and error() to check the object
  state after using this constructor.  */
Client::Client (const char *server,
                u_short port)
{
  initialized_ = 0;
  error_ = 0;
  this->open (server, port);
}

/* Open a connection to the server.  This hides the use of
  ACE_SOCK_Connector from our caller.  Since our caller probably
  doesn't care *how* we connect, this is a good thing.  */
int 
Client::open (const char *server,
              u_short port)
{
  /* This is right out of Tutorial 3.  The only thing we've added is
    to set the initialized_ member variable on success.  */

  ACE_SOCK_Connector connector;
  ACE_INET_Addr addr (port, server);

  if (connector.connect (*this, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  initialized_ = 1;
  return 0;
}

/* The first of our put operators sends a simple string object to the
  peer.  */
Client & 
Client::operator<< (ACE_SString &str)
{
  /* We have to be able to allow: server << foo << bar << stuff;

    To accomplish that, every << operator must check that the object
    is in a valid state before doing work.  */

  if (initialized () && !error ())
    {
      /* Get the actual data held in the string object */
      const char *cp = str.fast_rep ();

      /* Send that data to the peer using send_n() as before.  If we
        have a problem, we'll set error_ so that subsequent <<
        operations won't try to use a broken stream.  */
      if (this->send_n (cp,
                        ACE_OS::strlen (cp)) == -1)
        error_ = 1;
    }
  else
    /* Be sure that error_ is set if somebody tries to use us when
        we're not initialized.  */
    error_ = 1;

  /* We have to return a reference to ourselves to allow chaining of
    put operations (eg -- "server << foo << bar").  Without the
    reference, you would have to do each put operation as a statement.
    That's OK but doesn't have the same feel as standard C++
    iostreams.  */
  return *this ;
}

/* How do you put a char*?  We'll take an easy way out and construct
an ACE_SString from the char* and then put that.  It would have been
more efficient to implement this with the body of the
operator<<(ACE_SString&) method and then express that method in terms
of this one.  There's always more than one way to do things!  */

Client & 
Client::operator<< (char *str)
{
  ACE_SString newStr (str);

  *this << newStr;

  return *this ;

  /* Notice that we could have been really clever and done:

     return *this << ACE_SString (str);

    That kind of thing just makes debugging a pain though!  */
}

/* ACE_SString and char* are both about the same thing.  What do you
  do about different datatypes though?

  Do the same thing we did with char* and convert it to ACE_SString
  where we already have a << operator defined.  */
Client & 
Client::operator<< (int n)
{
  /* Create a character buffer large enough for the largest number.
    That's a tough call but BUFSIZ should be quite enough.  */
  char buf[BUFSIZ];

  /* Put the number into our buffer...  */
  ACE_OS::sprintf (buf,
                   "(%d)\n",
                   n);

  /* And create the ACE_SString that we know how to put.  */
  ACE_SString newStr (buf);

  /* Send it and...  */
  *this << newStr;

  /* return ourselves as usual.  */
  return *this;
}

/* Now we pull it all together.  Like Tutorial 3, we'll allow command
  line options.  */
int 
main (int argc, char *argv[])
{
  const char *server_host = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_HOST;
  u_short server_port = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_SERVER_PORT;
  int max_iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : 4;

  /* Use the basic constructor since the other isn't really very safe.  */
  Client peer;
  
  /* Open the server connection.  Notice how this is simpler than
    Tutorial 3 since we only have to provide a host name and port
    value.  */
  if (peer.open (server_host,
                 server_port) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  
  for (int i = 0; i < max_iterations; i++)
    {
      /* Tell the server which iteration we're on.  No more mucking
        aroudn with sprintf at this level!  It's all hidden from us.  */
      peer << "message = " << i+1;

      /* Everything OK?  */
      if (peer.error ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "send"),
                          -1);
      else
        ACE_OS::sleep (1);
    }

  if (peer.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                      -1);
  return 0;
}
