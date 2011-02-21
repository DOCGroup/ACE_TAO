// $Id$

// ============================================================================
//
// = LIBRARY
//    tests/SSL
//
// = FILENAME
//    SSL_Asynch_Stream_Test.cpp
//
// = DESCRIPTION
//      This program is a functionality test of ACE_SSL_Asynch_Stream.
//      It demonstrates one proper use case of ACE_SSL_Asynch_Stream in the
//      Proactor framework and validates its basic functionality.
//
//      Usage: SSL_Asynch_Stream_Test [-r <hostname:port#>]
//                [-t <num threads>] [-d <delay>]
//                [-i <client conn attempt#>] [-n <client request# per conn>]
//
//      Default value:
//          <hostname:port#>:       ACE_DEFAULT_SERVER_HOST:ACE_DEFAULT_PORT
//          <num threads>:          ACE_MAX_THREADS
//          <client conn attempt#>: ACE_MAX_ITERATIONS
//          <client req# per conn>: 20
//          <delay>:                0 usec
//
// = AUTHOR
//      Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "../test_config.h"
#include "ace/Default_Constants.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_string.h"
#include "ace/Event_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/Proactor.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "ace/INET_Addr.h"
#include "ace/SSL/SSL_Asynch_Stream.h"
#include "ace/SSL/SSL_SOCK_Connector.h"
#include "ace/SSL/SSL_SOCK_Acceptor.h"
#include "ace/SSL/SSL_SOCK_Stream.h"



#if defined (ACE_HAS_THREADS) && ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls.

class Client_Handler : public ACE_Handler
{
public:
  Client_Handler ()
    : msgs_sent_ (0),
      stream_ (ACE_SSL_Asynch_Stream::ST_CLIENT),
      block_ (1024) {}
  ~Client_Handler ();

  //FUZZ: disable check_for_lack_ACE_OS
  int open (ACE_HANDLE);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);

private:
  size_t                  msgs_sent_;
  ACE_SSL_Asynch_Stream   stream_;
  ACE_Message_Block       block_;
};

class Server_Handler : public ACE_Handler
{
public:
  Server_Handler ()
    : msgs_rcvd_ (0),
      stream_ (ACE_SSL_Asynch_Stream::ST_SERVER),
      block_ (1024) {}
  ~Server_Handler ();

  //FUZZ: disable check_for_lack_ACE_OS
  int open (ACE_HANDLE);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);

private:
  size_t                  msgs_rcvd_;
  ACE_SSL_Asynch_Stream   stream_;
  ACE_Message_Block       block_;
};

class Server_Acceptor : public ACE_Event_Handler
{
public:
  //FUZZ: disable check_for_lack_ACE_OS
  int open (const ACE_INET_Addr &listen_addr);
  //FUZZ: enable check_for_lack_ACE_OS

  // Get the I/O handle.
  virtual ACE_HANDLE get_handle (void) const;

  // Called when a new connection is ready to accept.
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

private:
  ACE_SSL_SOCK_Acceptor acceptor_;
};

// Accepting end point.  This is actually "localhost:10010", but some
// platform couldn't resolve the name so we use the IP address
// directly here.
static const ACE_TCHAR *rendezvous = \
  ACE_DEFAULT_SERVER_HOST ACE_TEXT (":") ACE_DEFAULT_SERVER_PORT_STR;

// Total number of proactor threads.
static size_t num_threads = ACE_MAX_THREADS;

#if defined (CHORUS) // Add platforms that can't handle too many
                     // connection simultaneously here.
#define ACE_LOAD_FACTOR /2
#else
#define ACE_LOAD_FACTOR
#endif

// Number of client connections to attempt.
static size_t cli_conn_no = ACE_MAX_ITERATIONS ACE_LOAD_FACTOR;

// Number of requests each client connection sends.
static size_t cli_req_no = ACE_MAX_THREADS ACE_LOAD_FACTOR;

// Delay before a thread sending the next request (in msec.)
static int req_delay = 0;

// This is the string sent from client to server.
static const char *test_string = "SSL_Asynch_Stream_Test!";

// Function to remove signals from the signal mask.
static int
disable_signal (int sigmin, int sigmax)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  sigset_t signal_set;
  if (ACE_OS::sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error: (%P|%t):%p\n"),
                ACE_TEXT ("sigemptyset failed")));

  for (int i = sigmin; i <= sigmax; i++)
    ACE_OS::sigaddset (&signal_set, i);

  // Put the <signal_set>.
# if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
  // In multi-threaded application this is not POSIX compliant
  // but let's leave it just in case.
  if (ACE_OS::sigprocmask (SIG_BLOCK, &signal_set, 0) != 0)
# else
  if (ACE_OS::thr_sigsetmask (SIG_BLOCK, &signal_set, 0) != 0)
# endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: (%P|%t): %p\n"),
                       ACE_TEXT ("SIG_BLOCK failed")),
                      -1);
#else
  ACE_UNUSED_ARG (sigmin);
  ACE_UNUSED_ARG (sigmax);
#endif /* ACE_LACKS_UNIX_SIGNALS */

  return 0;
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("r:t:d:i:n:"));

  int c;

  while ((c = getopt ()) != -1)
    {
  //FUZZ: enable check_for_lack_ACE_OS
      switch (c)
        {
        case 'r':                   // hostname:port
          rendezvous = getopt.opt_arg ();
          break;
        case 't':
          num_threads = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'd':
          req_delay = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'i':
          cli_conn_no = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'n':
          cli_req_no = ACE_OS::atoi (getopt.opt_arg ());
          break;
        default:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Usage: %s [-r <hostname:port#>]")
                      ACE_TEXT ("\t[-t <nr threads>] [-d <delay>]")
                      ACE_TEXT ("\t[-i <client conn attempt#>]")
                      ACE_TEXT ("\t[-n <client request# per conn>]\n"),
                      argv[0]));
          break;
        }
    }
}

Client_Handler::~Client_Handler ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Client %@ handle %d down\n"),
              this, this->stream_.handle ()));
  if (this->stream_.handle () != ACE_INVALID_HANDLE)
    {
      if (this->msgs_sent_ != cli_req_no)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%t) Client handle %d sent %d messages; ")
                    ACE_TEXT ("expected %d\n"),
                    this->stream_.handle (),
                    this->msgs_sent_,
                    cli_req_no));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Client handle %d sent %d messages; ")
                    ACE_TEXT ("closing connection\n"),
                    this->stream_.handle (),
                    cli_req_no));
    }
  this->stream_.close ();
}

int
Client_Handler::open (ACE_HANDLE handle)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Client %@ handle %d up\n"),
              this, handle));
  if (this->stream_.open (*this, handle) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) Client_Handler: %p\n"),
                       ACE_TEXT ("open")),
                      -1);
  this->block_.copy (test_string);
  if (this->stream_.write (this->block_, this->block_.length ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) Client_Handler: %p\n"),
                       ACE_TEXT ("initiate write")),
                      -1);
  return 0;
}

void
Client_Handler::handle_write_stream
  (const ACE_Asynch_Write_Stream::Result &result)
{
  if (!result.success ())
    {
      errno = result.error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) Client handle %d: %p\n"),
                  this->stream_.handle (),
                  ACE_TEXT ("write")));
      delete this;
      return;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Client %@ handle %d sent %B of %B bytes\n"),
              this, this->stream_.handle (),
              result.bytes_transferred (), result.bytes_to_write ()));
  ACE_Message_Block &b = result.message_block ();
  bool send_again = true;
  if (b.length () == 0)
    {
      // All block's data sent; rewind the read pointer and send it again
      // until we've sent the configured number of times.
      ++this->msgs_sent_;
      if (this->msgs_sent_ == cli_req_no)
        send_again = false;   // All done
      else
        b.rd_ptr (b.base ());
    }

  if (send_again)
    {
      if (this->stream_.write (this->block_, this->block_.length ()) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) Client_Handler: %p\n"),
                      ACE_TEXT ("initiate write")));
          delete this;
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Client handle %d done sending\n"),
                  this->stream_.handle ()));
      delete this;
    }
  return;
}

Server_Handler::~Server_Handler ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Server %@ handle %d down\n"),
              this, this->stream_.handle ()));
  if (this->stream_.handle () != ACE_INVALID_HANDLE)
    {
      if (this->msgs_rcvd_ != cli_req_no)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%t) Server handle %d received %d messages; ")
                    ACE_TEXT ("expected %d\n"),
                    this->stream_.handle (),
                    this->msgs_rcvd_,
                    cli_req_no));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Server handle %d received %d messages; ")
                    ACE_TEXT ("closing connection\n"),
                    this->stream_.handle (),
                    cli_req_no));
    }
  this->stream_.close ();
}

int
Server_Handler::open (ACE_HANDLE handle)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Server %@ handle %d up\n"),
              this, handle));
  if (this->stream_.open (*this, handle) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) Server_Handler: %p\n"),
                       ACE_TEXT ("open")),
                      -1);
  if (this->stream_.read (this->block_, this->block_.space () - 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) Server_Handler: %p\n"),
                       ACE_TEXT ("read")),
                      -1);
  return 0;
}

void
Server_Handler::handle_read_stream
  (const ACE_Asynch_Read_Stream::Result &result)
{
  if (!result.success ())
    {
      errno = result.error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) Server handle %d: %p\n"),
                  this->stream_.handle (),
                  ACE_TEXT ("read")));
      delete this;
      return;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Server %@ handle %d recv %B of %B bytes\n"),
              this, this->stream_.handle (),
              result.bytes_transferred (), result.bytes_to_read ()));
  if (result.bytes_transferred () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Server handle %d closed by peer\n"),
                  this->stream_.handle ()));
      delete this;
      return;
    }

  // Scan through the received data for the expected string. There may be
  // multiples and/or partials. Count up how many arrive before the connection
  // is closed.
  // Remember that the client side sends the terminating nul; in case the
  // whole thing didn't arrive, we add a nul to the end of the receive
  // block so we don't run off the end. When the recv into this buffer was
  // initiated, we left the last byte empty to facilitate this.
  ACE_Message_Block &b = result.message_block ();
  *(b.wr_ptr ()) = '\0';
  size_t test_string_len = ACE_OS::strlen (test_string);
  while (b.length () >= test_string_len)
    {
      if (0 != ACE_OS::strncmp (b.rd_ptr (), test_string, test_string_len))
        ACE_ERROR_BREAK ((LM_ERROR,
                          ACE_TEXT ("(%t) Read string: %C; expected: %C\n"),
                          b.rd_ptr (),
                          test_string));
      b.rd_ptr (test_string_len);
      // That ran up over the string; can we also consume the nul?
      if (b.length () > 0)
        b.rd_ptr (1);
      ++this->msgs_rcvd_;
    }
  b.crunch ();
  if (this->stream_.read (b, b.space () - 1) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) Server_Handler: %p\n"),
                  ACE_TEXT ("read")));
      delete this;
    }
  return;
}


int
Server_Acceptor::open (const ACE_INET_Addr &listen_addr)
{
  if (this->acceptor_.open (listen_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("listen")),
                      -1);
  return 0;
}

ACE_HANDLE
Server_Acceptor::get_handle (void) const
{
  return this->acceptor_.get_handle ();
}

int
Server_Acceptor::handle_input (ACE_HANDLE)
{
  ACE_SSL_SOCK_Stream new_stream;
  if (this->acceptor_.accept (new_stream) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("accept")),
                      -1);
  Server_Handler *new_handler = 0;
  ACE_NEW_RETURN (new_handler, Server_Handler, -1);
  if (new_handler->open (new_stream.get_handle ()) != 0)
    delete new_handler;

  return 0;
}

int
Server_Acceptor::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  this->acceptor_.close ();
  return 0;
}


static ACE_THR_FUNC_RETURN
proactor_loop (void *)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Start handling events.\n")));

  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);
  disable_signal (SIGPIPE, SIGPIPE);

  int result =
    ACE_Proactor::instance ()->proactor_run_event_loop ();
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("Error handling events")),
                      0);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Done handling events.\n")));

  return 0;
}

static ACE_THR_FUNC_RETURN
start_clients (void *)
{
  // Client thread function.
  ACE_INET_Addr addr (rendezvous);
  ACE_SSL_SOCK_Connector connect;

  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);
  disable_signal (SIGPIPE, SIGPIPE);

  for (size_t i = 0 ; i < cli_conn_no; i++)
    {
      ACE_SSL_SOCK_Stream stream;
      if (connect.connect (stream, addr) < 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p\n"),
                      ACE_TEXT ("connect")));
          continue;
        }

      Client_Handler *new_handler = 0;
      ACE_NEW_RETURN (new_handler, Client_Handler, (ACE_THR_FUNC_RETURN)-1);
      if (new_handler->open (stream.get_handle ()) != 0)
        {
          delete new_handler;
          stream.close ();
        }
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("SSL_Asynch_Stream_Test"));

  ACE_SSL_Context *context = ACE_SSL_Context::instance ();
  // Note - the next two strings are naked on purpose... the arguments to
  // the ACE_SSL_Context methods are const char *, not ACE_TCHAR *.
  context->certificate ("dummy.pem", SSL_FILETYPE_PEM);
  context->private_key ("key.pem", SSL_FILETYPE_PEM);

  parse_args (argc, argv);
  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);
  disable_signal (SIGPIPE, SIGPIPE);

  Server_Acceptor acceptor;
  ACE_INET_Addr accept_addr (rendezvous);

  if (acceptor.open (accept_addr) == -1)
    return 1;
  if (-1 == ACE_Reactor::instance ()->register_handler (&acceptor,
                                                        ACE_Event_Handler::ACCEPT_MASK))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p; aborting\n"),
                  ACE_TEXT ("register_handler")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Listening at %s\n"), rendezvous));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Spawning %d proactor threads\n"),
              num_threads));
  ACE_Thread_Manager::instance ()->spawn_n (num_threads, proactor_loop);
  ACE_Thread_Manager::instance ()->spawn (start_clients);

  ACE_Time_Value loop_limit (20);
  ACE_Reactor::instance ()->run_reactor_event_loop (loop_limit);
  ACE_Thread_Manager::instance ()->wait ();

  // Check for num connections up/down.

  ACE_END_TEST;
  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SSL_Asynch_Stream_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("This test requires threads and AIO which are not ")
              ACE_TEXT ("supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS && (WIN32 || AIO) */
