// $Id$

// This tests the features of the <ACE_SOCK_Connector> and
// <ACE_SOCK_Stream> classes.  In addition, it can be used to test the
// oneway and twoway latency and throughput at the socket-level.  This
// is useful as a baseline to compare against ORB-level performance
// for the same types of data.

#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Thread_Manager.h"
#include "ace/Singleton.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Synch.h"

ACE_RCSID(SOCK_SAP, CPP_inclient, "$Id$")

class Options
  // = TITLE
  //   Define the options for this test.
{
public:
  Options (void);
  // Constructor.

  ~Options (void);
  // Destructor.

  int parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

  const ACE_Time_Value &sleep_time (void) const;
  // Return the amount of time to sleep in order to implement the
  // proper transmission rates.

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
  int init (void);
  // Initialize the message we're sending to the user and set up the
  // barrier.

  char *shared_client_test (u_short port,
                            ACE_SOCK_Stream &cli_stream);
  // Performs the shared behavior of the oneway and twoway client
  // tests.

  static void *twoway_client_test (void *);
  // Performs the twoway test.

  static void *oneway_client_test (void *);
  // Performs the oneway test.

  const char *host_;
  // Host of the server.

  u_short port_;
  // Port of the server.

  ACE_Time_Value sleep_time_;
  // Sleep_Time value.

  size_t threads_;
  // Number of threads.

  const char *quit_string_;
  // String that shuts down the client/server.

  size_t message_len_;
  // Size of the message we send to the server.

  char *message_buf_;
  // Pointer to the message we send to the server.

  ACE_HANDLE io_source_;
  // Are we reading I/O from ACE_STDIN or from our generator?

  size_t iterations_;
  // Number of iterations.

  char oneway_;
  // Are we running oneway or twoway?

  ACE_MT (ACE_Barrier *barrier_);
  // Barrier used to synchronize the start of all the threads.
};

Options::Options (void)
  : host_ (ACE_DEFAULT_SERVER_HOST),
    port_ (ACE_DEFAULT_SERVER_PORT),
    sleep_time_ (0, 0), // By default, don't sleep between calls.
    threads_ (10),
    quit_string_ ("q"),
    message_len_ (0),
    message_buf_ (0),
    io_source_ (ACE_INVALID_HANDLE), // Defaults to using the generator.
    iterations_ (10000),
    oneway_ (1), // Make oneway calls the default.
    barrier_ (0)
{
}

Options::~Options (void)
{
  delete this->barrier_;
  delete [] this->message_buf_;
}
  
// Options Singleton.
typedef ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX> OPTIONS;

int
Options::init (void)
{
  // Check for default case.
  if (this->message_len_ == 0)
    this->message_len_ = ACE_OS::strlen ("TAO");

  this->message_len_ += sizeof (ACE_UINT32);

  ACE_NEW_RETURN (this->message_buf_,
                  char[this->message_len_],
                  -1);

  // Copy the length into the beginning of the message.
  ACE_UINT32 length = ntohl (this->message_len_);
  ACE_OS::memcpy ((void *) this->message_buf_,
                  (void *) &length,
                  sizeof length);

  ACE_OS::memset ((void *) (this->message_buf_ + sizeof (ACE_UINT32)),
                  'a',
                  this->message_len_ - sizeof (ACE_UINT32));

  // Allocate the barrier with the correct count.
  ACE_NEW_RETURN (this->barrier_,
                  ACE_Barrier (this->threads_),
                  -1);
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
  ACE_UNUSED_ARG (len);

  if (this->io_source_ == ACE_STDIN)
    return ACE_OS::read (ACE_STDIN, buf, len);
  else if (iteration >= this->iterations_)
    return 0;
  else
    {
      ACE_OS::memcpy (buf,
                      this->message_buf (),
                      len);
      iteration++;
      return len;
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
        this->sleep_time_.set (0, ACE_OS::atoi (getopt.optarg));
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) usage: %n [-2] [-h <host>] [-i iterations] [-m message-size] [-p <port>] [-q <quit string>] [-s] [-t <threads>] [-T <sleep_time>]\n"),
                          -1);
      }

  return this->init ();
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

const ACE_Time_Value &
Options::sleep_time (void) const
{
  return this->sleep_time_;
}

char *
Options::shared_client_test (u_short port,
                             ACE_SOCK_Stream &cli_stream)
{
  ACE_INET_Addr remote_addr (port, this->host_);

  ACE_SOCK_Connector con;

  if (con.connect (cli_stream,
                   remote_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "connection failed"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connected to %s at port %d\n",
                remote_addr.get_host_name (),
                remote_addr.get_port_number ()));

  ACE_INT32 len = htonl (this->message_len ());

  // Allocate the transmit buffer.
  char *buf;
  ACE_NEW_RETURN (buf,
                  char[len],
                  0);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) waiting...\n"));
              
  // Wait for all other threads to finish initialization.
  this->barrier_->wait ();
  return buf;
}
// Static function entry point to the oneway client service.

void *
Options::oneway_client_test (void *)
{
  Options *options = OPTIONS::instance ();
  ACE_SOCK_Stream cli_stream;

  // Add 1 to the port to trigger the oneway test!
  char *request = options->shared_client_test (options->port () + 1,
                                               cli_stream);
  if (request == 0)
    return 0;

  // This variable is allocated off the stack to obviate the need for
  // locking.
  size_t iteration = 0;

  // Keep track of return value.
  int result = 0;
  ACE_INT32 len = options->message_len ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting oneway transmission\n"));

  // Perform oneway transmission of data to server (correctly handles
  // "incomplete writes").

  for (ssize_t r_bytes;
       (r_bytes = options->read (request, len, iteration)) > 0;
       // Transmit at the proper rate.
       ACE_OS::sleep (options->sleep_time ()))
    if (ACE_OS::memcmp (request,
                        options->quit_string (),
                        ACE_OS::strlen (options->quit_string ())) == 0)
      break;
    else if (cli_stream.send_n (request, r_bytes) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n",
                    "send_n"));
        result = -1;
        break;
      }

  // Close the connection.
  cli_stream.close ();

  delete [] request;
  return (void *) result;
}

// Static function entry point to the twoway client service.

void *
Options::twoway_client_test (void *)
{
  Options *options = OPTIONS::instance ();

  ACE_SOCK_Stream cli_stream;

  char *request = options->shared_client_test (options->port (),
                                               cli_stream);
  if (request == 0)
    return 0;

  // This variable is allocated off the stack to obviate the need for
  // locking.
  size_t iteration = 0;

  // Keep track of return value.
  int result = 0;

  // Timer business.
  ACE_High_Res_Timer timer;

  ACE_INT32 len = options->message_len ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting twoway transmission\n"));

  // Perform twoway transmission of data to server (correctly handles
  // "incomplete writes").

  for (ssize_t r_bytes;
       (r_bytes = options->read (request, len, iteration)) > 0;
       // Transmit at the proper rate.
       ACE_OS::sleep (options->sleep_time ()))
    if (ACE_OS::memcmp (request,
                        options->quit_string (),
                        ACE_OS::strlen (options->quit_string ())) == 0)
      break;

    // Transmit <request> to the server.
    else
      {
        // Note that we use the incremental feature of the
        // <ACE_High_Res_Timer> so that we don't get "charged" for the
        // <ACE_OS::sleep> used to control the rate at which requests
        // are sent.
        timer.start_incr ();

        if (cli_stream.send_n (request, r_bytes) == -1)
          {
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) %p\n",
                        "send_n"));
            result = -1;
            break;
          }
        // Receive the reply from the server.  Normally, it just sends
        // back 24 bytes, which is typical for an IIOP reply.
        else if (cli_stream.recv (request, r_bytes) <= 0)
          {
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) %p\n",
                        "recv"));
            result = -1;
            break;
          }

        timer.stop_incr ();
      }

  ACE_Time_Value tv;

  timer.elapsed_time_incr (tv);
  double real_time = tv.sec () * ACE_ONE_SECOND_IN_USECS + tv.usec ();
  double messages_per_sec = iteration * double (ACE_ONE_SECOND_IN_USECS) / real_time;

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("(%t) messages = %d\n(%t) usec-per-message = %f\n(%t) messages-per-second = %0.00f\n"),
	      iteration,
	      real_time / double (iteration),
              messages_per_sec < 0 ? 0 : messages_per_sec));

  // Close the connection.
  cli_stream.close ();

  delete [] request;
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

static int
run_client (void)
{
  // Raise the socket handle limit to the maximum.
  ACE::set_handle_limit ();

#if defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn_n (OPTIONS::instance ()->threads (),
                                                OPTIONS::instance ()->thr_func ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "spawn_n"),
                      1);
  else
    ACE_Thread_Manager::instance ()->wait ();
#else
  client (0);
#endif /* ACE_HAS_THREADS */
  return 0;
}

int
main (int argc, char *argv[])
{
  // Initialize the logger.
  ACE_LOG_MSG->open (argv[0]);

  if (OPTIONS::instance ()->parse_args (argc, argv) == -1)
    return -1;

  // Run the client
  run_client ();

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
