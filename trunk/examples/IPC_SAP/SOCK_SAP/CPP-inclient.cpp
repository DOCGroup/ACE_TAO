// $Id$

// This tests the non-blocking features of the ACE_SOCK_Connector
// class.

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

  ssize_t read (char *buf, size_t len, size_t &iterations);
  // Read from the appropriate location.

private:
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

  ACE_HANDLE io_source_;
  // Are we reading I/O from ACE_STDIN or from our generator?

  size_t iterations_;
  // Number of iterations.
};

Options::Options (void)
  : host_ (ACE_DEFAULT_SERVER_HOST),
    port_ (ACE_DEFAULT_SERVER_PORT),
    timeout_ (ACE_DEFAULT_TIMEOUT),
    threads_ (10),
    quit_string_ ("quit"),
    io_source_ (ACE_INVALID_HANDLE), // Defaults to using the generator.
    iterations_ (10000)
{
}

ssize_t
Options::read (char *buf, size_t len, size_t &iteration)
{
  if (io_source_ == ACE_STDIN)
    return ACE_OS::read (ACE_STDIN, buf, sizeof buf);
  else if (iteration >= this->iterations_)
    return 0;
  else
    {
      ACE_OS::strncpy (buf, "TAO", len);
      iteration++;
      return ACE_OS::strlen ("TAO") + 1;
    }
}

int
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt getopt (argc, argv, "h:i:p:q:st:T:", 1);

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'h':
        this->host_ = getopt.optarg;
        break;
      case 'i':
        this->iterations_ = ACE_OS::atoi (getopt.optarg);
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
                           "(%P|%t) usage: %s [-h <host>] [-p <port>] [-q <quit string>] [-s] [-t <threads>] [-T <timeout>]"),
                          1);
      }

  return 0;
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

// Options Singleton.
typedef ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX> OPTIONS;

// Entry point to the client service.

void *
client (void *)
{
  char buf[BUFSIZ];

  Options *options = OPTIONS::instance ();

  ACE_SOCK_Stream cli_stream;
  ACE_INET_Addr remote_addr (options->port (),
                             options->host ());

  ACE_SOCK_Connector con;

  // Initiate blocking connection with server.
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting connect\n"));

  if (con.connect (cli_stream, remote_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "connection failed"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connected to %s\n",
                remote_addr.get_host_name ()));

  // This variable is allocated off the stack to obviate the need for
  // locking.
  size_t iteration = 0;

  // Send data to server (correctly handles "incomplete writes").

  for (ssize_t r_bytes;
       (r_bytes = options->read (buf, sizeof buf, iteration)) > 0;
       )
    if (ACE_OS::strncmp (buf,
                         options->quit_string (),
                         ACE_OS::strlen (options->quit_string ())) == 0)
      break;
    else if (cli_stream.send (buf, r_bytes, 0) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) %p\n",
                         "send_n"),
                        0);
    else if (cli_stream.recv (buf, sizeof buf) <= 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) %p\n",
                         "recv"),
                        0);

  // Close the connection completely.
  if (cli_stream.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "close"),
                      0);
  return 0;
}

int
main (int argc, char *argv[])
{
  OPTIONS::instance ()->parse_args (argc, argv);

#if defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn_n (OPTIONS::instance ()->threads (),
                                                (ACE_THR_FUNC) client) == -1)
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
