// $Id$

// This tests the features of the <ACE_SSL_SOCK_Connector> and
// <ACE_SSL_SOCK_Stream> classes.  In addition, it can be used to test the
// oneway and twoway latency and throughput at the socket-level.  This
// is useful as a baseline to compare against ORB-level performance
// for the same types of data.

#include "ace/INET_Addr.h"
#include "ace/Singleton.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

#include "ace/SSL/SSL_SOCK_Connector.h"

#include "SSL-client-simple.h"

ACE_RCSID(SSL_SAP, SSL_client_simple, "$Id$")

Options::Options (void)
  : host_ (ACE_DEFAULT_SERVER_HOST),
    port_ (ACE_DEFAULT_SERVER_PORT),
    sleep_time_ (0, 0), // By default, don't sleep between calls.
    quit_string_ ("q"),
    message_len_ (0),
    message_buf_ (0),
    io_source_ (ACE_INVALID_HANDLE), // Defaults to using the generator.
    iterations_ (10000),
    oneway_ (1) // Make oneway calls the default.
{
}

Options::~Options (void)
{
  delete [] this->message_buf_;
}

// Options Singleton.
typedef ACE_Singleton<Options, ACE_Null_Mutex> OPTIONS;

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
  ACE_Get_Opt getopt (argc, argv, "2h:i:m:p:q:sT:", 1);

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case '2': // Disable the oneway client.
        this->oneway_ = 0;
        break;
      case 'h':
        this->host_ = getopt.opt_arg ();
        break;
      case 'i':
        this->iterations_ = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'm':
        this->message_len_ = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'p':
        this->port_ = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'q':
        this->quit_string_ = getopt.opt_arg ();
        break;
      case 's':
        this->io_source_ = ACE_STDIN;
        break;
      case 'T':
        this->sleep_time_.set (0, ACE_OS::atoi (getopt.opt_arg ()));
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) usage: %n [-2] [-h <host>] "
                           "[-i iterations] [-m message-size] "
                           "[-p <port>] [-q <quit string>] "
                           "[-s] [-T <sleep_time>]\n"),
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

const ACE_Time_Value &
Options::sleep_time (void) const
{
  return this->sleep_time_;
}

char *
Options::shared_client_test (u_short port,
                             ACE_SSL_SOCK_Stream &cli_stream)
{
  ACE_INET_Addr remote_addr (port, this->host_);

  ACE_SSL_SOCK_Connector con;

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

  char *buf;
  ACE_NEW_RETURN (buf,
                  char[this->message_len ()],
                  0);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) waiting...\n"));

  return buf;
}
// Static function entry point to the oneway client service.

void
Options::oneway_client_test (void)
{
  ACE_SSL_SOCK_Stream cli_stream;

  // Add 1 to the port to trigger the oneway test!
  char *request = this->shared_client_test (this->port () + 1,
                                            cli_stream);
  if (request == 0)
    return;

  // This variable is allocated off the stack to obviate the need for
  // locking.
  size_t iteration = 0;

  // Keep track of return value.
  int result = 0;
  ACE_INT32 len = this->message_len ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting oneway transmission\n"));

  // Perform oneway transmission of data to server (correctly handles
  // "incomplete writes").

  for (ssize_t r_bytes;
       (r_bytes = this->read (request, len, iteration)) > 0;
       // Transmit at the proper rate.
       ACE_OS::sleep (this->sleep_time ()))
    if (ACE_OS::memcmp (request,
                        this->quit_string (),
                        ACE_OS::strlen (this->quit_string ())) == 0)
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
}

// Static function entry point to the twoway client service.

void
Options::twoway_client_test (void)
{
  ACE_SSL_SOCK_Stream cli_stream;

  char *request = this->shared_client_test (this->port (),
                                            cli_stream);
  if (request == 0)
    return;

  // This variable is allocated off the stack to obviate the need for
  // locking.
  size_t iteration = 0;

  // Keep track of return value.
  int result = 0;

  // Timer business.
  ACE_High_Res_Timer timer;

  ACE_INT32 len = this->message_len ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting twoway transmission\n"));

  // Perform twoway transmission of data to server (correctly handles
  // "incomplete writes").

  for (ssize_t r_bytes;
       (r_bytes = this->read (request, len, iteration)) > 0;
       // Transmit at the proper rate.
       ACE_OS::sleep (this->sleep_time ()))
    if (ACE_OS::memcmp (request,
                        this->quit_string (),
                        ACE_OS::strlen (this->quit_string ())) == 0)
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
              ACE_TEXT ("(%t) messages = %d\n(%t) usec-per-message = %f\n(%t) messages-per-second = %0.00f\n"),
              iteration,
              real_time / double (iteration),
              messages_per_sec < 0 ? 0 : messages_per_sec));

  // Close the connection.
  cli_stream.close ();

  delete [] request;
}

void
Options::run (void)
{
  if (this->oneway_ == 0)
    this->twoway_client_test ();
  else
    this->oneway_client_test ();
}

static int
run_client (void)
{
  // Raise the socket handle limit to the maximum.
  ACE::set_handle_limit ();

  OPTIONS::instance ()->run ();

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
template class ACE_Singleton<Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
