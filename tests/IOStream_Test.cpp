
//=============================================================================
/**
 *  @file    IOStream_Test.cpp
 *
 *  $Id$
 *
 *  This is a simple test of the IOStream class that illustrates
 *  how to use iostream operations on almost arbitrary I/O classes.
 *
 *
 *  @author James CE Johnson <jcej@lads.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Thread.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/IOStream.h"
#include "ace/OS_NS_sys_wait.h"



#if !defined (ACE_LACKS_ACE_IOSTREAM)
#  include "ace/OS_NS_unistd.h"
#  include "ace/OS_NS_ctype.h"  // Needed for isspace() function

typedef ACE_IOStream<ACE_SOCK_Stream> ACE_SOCK_IOStream;

/* The biggest drawback to an iostream is that it generally
   eats up whitespace when performing a get (>>) operation.

   That may be good if you're reading non-textual data but
   if you're trying to read a stream of words with embedded
   whitespace, it isn't going to be pleasant.

   If you've been blessed with the GNU String class, I've
   already provided a derived class, QuotedString, that
   makes dealing with strings very easy.

   If you're stuck with an array of characters then you
   will probably need somthing like I have below.

   On the other hand, one of the biggest advantages to an
   iostream is that it eats up whitespace :-)

   If you put (<<) your non-textual data to the iostream
   with any number of whitespace between the data then
   you can easily get (>>) the data from the iostream
   without having to worry about delimeters and such.

   The main thing to keep in mind when using an iostream
   between peers is that you MUST keep the data "fields"
   in sync.  That is, if the "putter" puts an int followed
   by a float followed by a double, you must make sure
   that the "getter" will be attempting to get an int
   then a float then a double.
 */

// Since I can't rely on GNU's String class being everywhere (yet),
// here's a simple class that will work with quoted strings.  Use at
// your own risk!  It is very incomplete!

class qchar
{
public:
  qchar (void) { c_ = '\0'; }

  qchar (char c) : c_ (c) { };

  operator char () const { return c_; }

  qchar operator= (char c) { return c_ = c; }

  bool operator== (char c) { return c_ == c; }

  friend ACE_SOCK_IOStream &operator>> (ACE_SOCK_IOStream & stream, qchar * buf);
  friend ACE_SOCK_IOStream &operator<< (ACE_SOCK_IOStream & stream, qchar * buf);
  friend ostream &operator<< (ostream & stream, qchar * buf);

private:
  char c_;
};

// This is taken almost directly from the QuotedString object that has
// been derived from GNU's String class.  The advantage to using
// QuotedString is that it is MUCH more robust than qchar will every
// be.

ACE_SOCK_IOStream &
operator>> (ACE_SOCK_IOStream & stream, qchar *buf)
{
  char c;

  *buf = '\0';  // Initialize the string

  stream.get (c);

  if (!stream) // eat space up to the first char
    return stream;

  // if we don't have a quote, append until we see space
  if (c != '"')
    for (*buf++ = c;
         (void *) stream.get (c) && !ACE_OS::ace_isspace (c);
         *buf++ = c)
      continue;
  else
    for (; (void *) stream.get (c) && c != '"'; *buf++ = c)
      if (c == '\\')
        {
          stream.get (c);
          if (c != '"')
            *buf++ = '\\';
        }

  *buf = '\0';

  return stream;
}

ACE_SOCK_IOStream &
operator<< (ACE_SOCK_IOStream &stream, qchar *buf)
{
  stream.put ('"');

  while (*buf)
    {
      if (*buf == '"')
        stream.put ('\\');

      stream.put ((char) *buf++);
    }

  stream.put ('"');

  return stream;
}

ostream &
operator<< (ostream &stream, qchar *buf)
{
  while (*buf)
    stream.put ((char) *buf++);

  return stream;
}

// Our client thread will initiate the test by sending some data to
// the server.

static void *
client (void *arg = 0)
{
  ACE_UNUSED_ARG (arg);

  // We don't _need_ to dynamically allocate the ACE_SOCK_IOStream.
  // But if we don't, it doesn't get destroyed on some platforms,
  // e.g., g++ 2.7.2.1 and Sun C++ 4.2 on Solaris 2.5.1. (It does work
  // on Linux, so the code seems fine.)  If we manage the storage
  // ourselves, we _will_ destroy it at the end of this function.
  ACE_SOCK_IOStream server;

  ACE_INET_Addr *remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr addr (remote_addr->get_port_number (),
                      ACE_DEFAULT_SERVER_HOST);
  ACE_SOCK_Connector connector;

  if (connector.connect (server, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%t) %p\n"),
                       ACE_TEXT ("Failed to connect to server thread")),
                      0);

  // Send a string to the server which it can interpret as a qchar[]
  const char *str = "\"This is a test     string.\"";
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) Client Sending: (%C)\n"),
              str));
  server << str << endl;

  // Allow the server to get the string and echo it to the user. (The
  // iostream doesn't need this, but humans do :)
  ACE_OS::sleep (2);

  // Send another string but this time the server will read it as a
  // char[].  Notice how the server's output doesn't include all of
  // the spaces sent by the client.

  str = "\"THIS IS A     TEST STRING.\"";
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Client Sending: (%s)\n"),
              str));
  server << str << endl;

  // Again, give the server time to display the happenings to the
  // user.
  ACE_OS::sleep (2);

  // Read from the server an int, float, long, float double.  The
  // iostream will pull them out by using the whitespace provided by
  // the server.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Client Receiving\n")));

  ACE_Time_Value timeout (2);
  ACE_Time_Value *timeoutp = &timeout;

  server >> timeoutp;

  int i;
  float f1, f2;
  long l;
  double d;

  while (! (server >> i))
    {
      int eof = server.eof ();
      if (eof)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT (" (%P|%t) Unrecoverable stream error/eof\n")));
          break;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT (" (%P|%t) Recoverable stream error/timed out)\n")));
          server.clear (0);
        }
    }

  server >> f1;
  server >> l;
  server >> f2;
  server >> d;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Client Received: int %d float %f long %d float %f double %f\n"),
              i,
              f1,
              (int) l,
              f2,
              d));

  // Check for proper received values.
  ACE_TEST_ASSERT (i == 1  && (f1 >= 0.123420 && f1 <= 0.123422)
              && l == 666555444  && (f2 >= 23.44 && f2 <= 23.46)
              && (d >= -47.1e+9 && d <= -45.9e+9));
  // Reset the precision to limit ourselves to two significant digits.
  server.precision (2);

  // Now, make a little change & send 'em back.
  i  *= -1; server << i  << " ";
  f1 *= -1.0; server << f1 << " ";
  l  *= -1; server << l  << " ";
  f2 *= -1.0; server << f2 << " ";
  d  *= -1; server << d  << " ";
  server << endl;

  // Shut down the test.
  server.close ();

  return 0;
}

// Test the server's ability to receive data from the client and then
// begin a two-way conversation.

static void *
server (void *arg = 0)
{
  // We don't _need_ to dynamically allocate the ACE_SOCK_IOStream.
  // But if we don't, it doesn't get destroyed on some platforms,
  // e.g., g++ 2.7.2.1 and Sun C++ 4.2 on Solaris 2.5.1. (It does work
  // on Linux, so the code seems fine.)  If we manage the storage
  // ourselves, we _will_ destroy it at the end of this function.
  ACE_SOCK_IOStream client_handler;

  ACE_INET_Addr server_addr;
  ACE_SOCK_Acceptor *acceptor =
    reinterpret_cast<ACE_SOCK_Acceptor *> (arg);

  if (acceptor->get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("get_local_addr")),
                      0);

#if defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (client),
                                              (void *) &server_addr,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%t) %p\n"),
                       ACE_TEXT ("spawing client thread")),
                      0);
#endif /* ACE_HAS_THREADS */

  if (acceptor->accept (client_handler) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Failed to accept new client_handler")),
                      0);

  // Read a qbuf[] from the client.  Notice that all of the client's
  // whitespace is preserved.
  qchar qbuf[BUFSIZ];
  ACE_OS::memset (qbuf, 0, sizeof qbuf);
  client_handler >> qbuf;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Server Received: (\"%C\")\n"),
              (char *) qbuf));

  // Give the client time to announce the next test to the user.
  ACE_OS::sleep (2);

  // Now we try to use a char[] to get a string from the client.
  // Compared to the method above, this is quite messy.  Notice also
  // that whitespace is lost.

#if defined (ACE_HAS_STRING_CLASS) && defined (ACE_HAS_STANDARD_CPP_LIBRARY)
  ACE_IOStream_String buf;
  ACE_DEBUG ((LM_DEBUG,
              " (%P|%t) Server Received: ("));

  while (client_handler &&
 (buf.length () == 0 || buf[buf.length () - 1] != '"'))
    {
      if (! (client_handler >> buf))
        break;

      if (buf.length () > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "%s ",
                    buf.c_str ()));
    }

  ACE_DEBUG ((LM_DEBUG,
              ")\n"));
#else
  char buf[BUFSIZ];
  ACE_OS::memset (buf, 0, sizeof buf);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Server Received: (")));

  while (ACE_OS::strlen (buf) == 0
         || buf[ACE_OS::strlen (buf) - 1] != '"')
    {
      if (! (client_handler >> buf))
        break;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%C "),
                  buf));
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (")\n")));
#endif /* ACE_HAS_STRING_CLASS */

  // Send some non-textual data to the client.  We use a single
  // character to separate the fields but we could have used any valid
  // whitespace.  The data will be sent if the iostream's buffer gets
  // filled or when we flush it with an explicit client.sync ()
  // command or the implicit <<endl.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Server sleeping\n")));
  ACE_OS::sleep (5);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Server Sending:  1 .12342134 666555444 23.45 -46.5e9\n")));
  client_handler << 1 << " ";
  client_handler << .12342134 << " ";
  client_handler << 666555444 << " ";
  client_handler << 23.45 << " ";
  client_handler << -46.5e9 << " ";
  client_handler << endl;

  // The client will have changed the sign of each data field and sent
  // 'em all back to us.  At the same time, the client used the
  // precision () function to change the significant digits for
  // non-integer values.
  int i;
  float f1, f2;
  long l;
  double d;
  client_handler >> i >> f1 >> l >> f2 >> d;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) Server Received: int %d float %f long %d float %f double %f\n"),
              i,
              f1,
              (int) l,
              f2,
              d));

  // check for proper received values
  ACE_TEST_ASSERT (i == -1  && (f1 >= -0.13 && f1 <= -0.11)
              && l == -666555444  && (f2 >= -24.0 && f2 <= -22.0)
              && (d >= 45e+9 && d <= 47e+9));

  client_handler.close ();

  return 0;
}

static int
spawn (void)
{
  // Acceptor;
  ACE_SOCK_Acceptor acceptor;

  if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) %p\n"),
                       ACE_TEXT ("open")),
                      -1);
#if defined (ACE_HAS_THREADS)
  else if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (server),
                                                  &acceptor,
                                                  THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawning server thread")),
                      -1);

  // Wait for the client and server thread to exit.
  ACE_Thread_Manager::instance ()->wait ();

#elif !defined (ACE_LACKS_FORK)

  switch (ACE_OS::fork ("child"))
    {
    case -1:
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n%a"),
                  ACE_TEXT ("fork failed")));
      ACE_OS::_exit (-1);
    case 0: // In child
      {
        ACE_APPEND_LOG ("IOStream_Test-children");
        ACE_INET_Addr server_addr;

        if (acceptor.get_local_addr (server_addr) == -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("get_local_addr")));
        else
          client ((void *) &server_addr);
        ACE_END_LOG;
        break;
      }
    default: // In parent
      server (&acceptor);

      // Allow the client to exit, then remove the Process_Mutex.
      ACE_OS::wait ();
      break;
    }
#else
  ACE_ERROR_RETURN ((LM_INFO,
                     ACE_TEXT ("threads *and* processes not supported on this platform\n")),
                    -1);
#endif /* ACE_HAS_THREADS */

  acceptor.close ();

  return 0;
}
#endif /* !ACE_LACKS_ACE_IOSTREAM */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("IOStream_Test"));

#if !defined (ACE_LACKS_ACE_IOSTREAM)
  ACE_INIT_LOG (ACE_TEXT ("IOStream_Test-children"));
  spawn ();
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("ACE_IOSTREAM not supported on this platform\n")));
#endif /* !ACE_LACKS_ACE_IOSTREAM */
  ACE_END_TEST;
  return 0;
}

