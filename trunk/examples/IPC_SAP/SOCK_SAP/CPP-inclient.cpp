// $Id$

// This tests the features of the ACE_SOCK_Connector and
// ACE_SOCK_Stream classes.

#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Thread_Manager.h"
#include "ace/Singleton.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"

// ACE SOCK_SAP client.

class Options
  // = TITLE
  //   Define the options for this test.
{
public:
  Options (void);
  // Constructor.

  int parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

  ACE_Time_Value *timeout (void) const;
  // Timeout value.

  u_short port (void) const;
  // Port of the server.

  const char *host (void) const;
  // Host of the server.

  size_t threads (void) const;
  // Number of threads.

  const char *quit_string (void) const;
  // String that shuts down the client/server.

  ssize_t read (void *buf, size_t len, size_t &iterations);
  // Read from the appropriate location.

  size_t message_len (void) const;
  // Returns the length of the message to send.

  const void *message_buf (void) const;
  // Returns a pointer to the message.

  ACE_THR_FUNC thr_func (void);
  // Returns a pointer to the entry point into the thread that runs
  // the client test function.

private:
  int initialize_message (void);
  // Initialize the message we're sending to the user.

  static void *twoway_client_test (void *);
  // Performs the twoway test.

  static void *oneway_client_test (void *);
  // Performs the oneway test.

  const char *host_;
  // Host of the server.

  u_short port_;
  // Port of the server.

  ACE_Time_Value timeout_;
  // Timeout value.

  size_t threads_;
  // Number of threads.

  const char *quit_string_;
  // String that shuts down the client/server.

  size_t message_len_;
  // Size of the message we send to the server.

  void *message_buf_;
  // Pointer to the message we send to the server.

  ACE_HANDLE io_source_;
  // Are we reading I/O from ACE_STDIN or from our generator?

  size_t iterations_;
  // Number of iterations.

  char oneway_;
  // Are we running oneway or twoway?
};

Options::Options (void)
  : host_ (ACE_DEFAULT_SERVER_HOST),
    port_ (ACE_DEFAULT_SERVER_PORT),
    timeout_ (ACE_DEFAULT_TIMEOUT),
    threads_ (10),
    quit_string_ ("q"),
    message_len_ (0),
    message_buf_ (0),
    io_source_ (ACE_INVALID_HANDLE), // Defaults to using the generator.
    iterations_ (10000),
    oneway_ (1) // Make oneway calls the default.
{
}

// Options Singleton.
typedef ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX> OPTIONS;

int
Options::initialize_message (void)
{
  // Check for default case.
  if (this->message_len_ == 0)
    {
      ACE_ALLOCATOR_RETURN (this->message_buf_,
                            ACE_OS::strdup ("TAO"),
                            -1);
      this->message_len_ = ACE_OS::strlen ("TAO");
    }
  else
    {
      ACE_ALLOCATOR_RETURN (this->message_buf_,
                            ACE_OS::malloc (this->message_len_),
                            -1);

      ACE_OS::memset (this->message_buf_,
                      'a',
                      this->message_len_);
    }

  return 0;
}

size_t
Options::message_len (void) const
{
  return this->message_len_;
}

const void *
Options::message_buf (void) const
{
  return this->message_buf_;
}

ssize_t
Options::read (void *buf, size_t len, size_t &iteration)
{
  if (this->io_source_ == ACE_STDIN)
    return ACE_OS::read (ACE_STDIN, buf, sizeof buf);
  else if (iteration >= this->iterations_)
    return 0;
  else
    {
      size_t size = this->message_len ();
      ACE_OS::memcpy (buf,
                      this->message_buf (),
                      size);
      iteration++;
      return size;
    }
}

int
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt getopt (argc, argv, "2h:i:m:p:q:st:T:", 1);

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case '2': // Disable the oneway client.
        this->oneway_ = 0;
        break;
      case 'h':
        this->host_ = getopt.optarg;
        break;
      case 'i':
        this->iterations_ = ACE_OS::atoi (getopt.optarg);
        break;
      case 'm':
        this->message_len_ = ACE_OS::atoi (getopt.optarg);
        break;
      case 'p':
        this->port_ = ACE_OS::atoi (getopt.optarg);
        break;
      case 'q':
        this->quit_string_ = getopt.optarg;
        break;
      case 's':
        this->io_source_ = ACE_STDIN;
        break;
      case 't':
        this->threads_ = (size_t) ACE_OS::atoi (getopt.optarg);
        break;
      case 'T':
        this->timeout_ = ACE_OS::atoi (getopt.optarg);
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) usage: %n [-2] [-h <host>] [-i iterations] [-m message-size] [-p <port>] [-q <quit string>] [-s] [-t <threads>] [-T <timeout>]"),
                          -1);
      }

  return this->initialize_message ();
}

u_short
Options::port (void) const
{
  return this->port_;
}

const char *
Options::host (void) const
{
  return this->host_;
}

const char *
Options::quit_string (void) const
{
  return this->quit_string_;
}

size_t
Options::threads (void) const
{
  return this->threads_;
}

ACE_Time_Value *
Options::timeout (void) const
{
  return (ACE_Time_Value *) &this->timeout_;
}

// Static function entry point to the oneway client service.

void *
Options::oneway_client_test (void *)
{
  Options *options = OPTIONS::instance ();

  ACE_SOCK_Stream cli_stream;
  // Add one to the port for the oneway test!
  ACE_INET_Addr remote_addr (options->port () + 1,
                             options->host ());

  ACE_SOCK_Connector con;

  // Initiate blocking connection with server.
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting oneway connect\n"));

  if (con.connect (cli_stream, remote_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "connection failed"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connected to %s at port %d\n",
                remote_addr.get_host_name (),
                remote_addr.get_port_number ()));

  ACE_UINT32 len = htonl (options->message_len ());

  if (cli_stream.send_n ((void *) &len,
                         sizeof (ACE_UINT32)) != sizeof (ACE_UINT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "send_n failed"),
                      0);

  // Allocate the transmit buffer.
  void *buf;
  ACE_ALLOCATOR_RETURN (buf, 
                        ACE_OS::malloc (len),
                        0);

  // This variable is allocated off the stack to obviate the need for
  // locking.
  size_t iteration = 0;

  // Keep track of return value.
  int result = 0;

  // Perform oneway transmission of data to server (correctly handles
  // "incomplete writes").

  for (ssize_t r_bytes;
       (r_bytes = options->read (buf, sizeof buf, iteration)) > 0;
       )
    if (ACE_OS::memcmp (buf,
                        options->quit_string (),
                        ACE_OS::strlen (options->quit_string ())) == 0)
      break;
    else if (cli_stream.send (buf, r_bytes, 0) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n",
                    "send_n"));
        result = -1;
        break;
      }

  // Close the connection.
  cli_stream.close ();

  ACE_OS::free (buf);
  return (void *) result;
}

// Static function entry point to the twoway client service.

void *
Options::twoway_client_test (void *)
{
  Options *options = OPTIONS::instance ();

  ACE_SOCK_Stream cli_stream;
  ACE_INET_Addr remote_addr (options->port (),
                             options->host ());

  ACE_SOCK_Connector con;

  // Initiate blocking connection with server.
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting twoway connect\n"));

  if (con.connect (cli_stream, remote_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "connection failed"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connected to %s at port %d\n",
                remote_addr.get_host_name (),
                remote_addr.get_port_number ()));

  ACE_UINT32 len = htonl (options->message_len ());

  if (cli_stream.send_n ((void *) &len,
                         sizeof (ACE_UINT32)) != sizeof (ACE_UINT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "send_n failed"),
                      0);

  // Allocate the transmit buffer.
  void *buf;
  ACE_ALLOCATOR_RETURN (buf, 
                        ACE_OS::malloc (len),
                        0);

  // This variable is allocated off the stack to obviate the need for
  // locking.
  size_t iteration = 0;

  // Keep track of return value.
  int result = 0;

  // Perform twoway transmission of data to server (correctly handles
  // "incomplete writes").

  for (ssize_t r_bytes;
       (r_bytes = options->read (buf, len, iteration)) > 0;
       )
    if (ACE_OS::memcmp (buf,
                        options->quit_string (),
                        ACE_OS::strlen (options->quit_string ())) == 0)
      break;
    else if (cli_stream.send (buf, r_bytes, 0) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n",
                    "send_n"));
        result = -1;
        break;
      }
    else if (cli_stream.recv (buf, r_bytes) <= 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n",
                    "recv"));
        result = -1;
        break;
      }

  // Close the connection.
  cli_stream.close ();

  ACE_OS::free (buf);
  return (void *) result;
}

ACE_THR_FUNC
Options::thr_func (void)
{
  if (this->oneway_ == 0)
    return ACE_THR_FUNC (&Options::twoway_client_test);
  else
    return ACE_THR_FUNC (&Options::oneway_client_test);
}

int
main (int argc, char *argv[])
{
  // Initialize the logger.
  ACE_LOG_MSG->open (argv[0]);

  if (OPTIONS::instance ()->parse_args (argc, argv) == -1)
    return -1;

#if defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn_n (OPTIONS::instance ()->threads (),
                                                OPTIONS::instance ()->thr_func ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "spawn_n"), 1);
  else
    ACE_Thread_Manager::instance ()->wait ();
#else
  client (0);
#endif /* ACE_HAS_THREADS */

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
