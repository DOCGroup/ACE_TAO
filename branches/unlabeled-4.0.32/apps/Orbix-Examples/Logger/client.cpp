// A client for the distributed logger example.  This program reads
// @(#)client.cpp	1.1	10/18/96

// from either stdin or from a redirected file and sends all the
// contents to the logging server.  It also computes how long it takes
// to send this stuff. 

#include "ace/Log_Msg.h"
#include "Logger.h"

// maximum message size
static size_t max_message_size = BUFSIZ;

// Default behavior is to use the locator service.
static char *hostname = 0;

// Should we prompt the user?
static int user_prompt;

static void
parse_args (int argc, char *argv[])
{
  extern char *optarg;
  extern int  optind;
  int c;

  ACE_LOG_MSG->open (argv[0]);

  // If a file has been redirected, don't activate user prompts
  if (ACE_OS::isatty (0))
    user_prompt = 1;
  else
    user_prompt = 0;

  while ((c = ACE_OS::getopt (argc, argv, "m:h:")) != -1)
    switch (c)
      {
      case 'm':
        max_message_size = ACE_OS::atoi (optarg) * BUFSIZ;
        break;
      case 'h':
	hostname = optarg;
	break;
      default:
        ACE_ERROR ((LM_ERROR, "%n: -h host -m max_message_size (in kbytes)\n%a", 1));
	/* NOTREACHED */
      }
}

// Enable/disable verbose logging.

static int
toggle_verbose (Logger &logger)
{
  int verbose_value;

  verbose_value = logger.verbose ();
  logger.verbose (!verbose_value);
  return 0;
}

// Transmit messages to the server.

int
transmit (Logger &logger, char buf[], ACE_HANDLE handle = 0)
{
  if (user_prompt)
    cout << "\nEnter message ('Q':quit,'V':toggle verbose):\n" << flush;

  ssize_t nbytes = ACE_OS::read (handle, buf, max_message_size);

  if (nbytes <= 0)
    return nbytes; // End of file or error.
  buf[nbytes] = '\0';

  if (user_prompt)
    {
      if (buf[0] == 'Q' || buf[0] == 'q')
	return 0;
      // toggle verbose?
      else if (buf[0] == 'V' || buf[0] == 'v')
	toggle_verbose (logger);
    }

  // send the message to the logger
  if (logger.log (logger::LM_DEBUG, buf, nbytes) == -1)
    return -1;
  else
    return nbytes;
}

// Print the results of the tests.

void
report_results (profile_logger::Elapsed_Time &et, size_t total_bytes)
{
  ACE_DEBUG ((LM_DEBUG, 
	     "real time = %8.2f\n"
	     "user time = %8.2f\n"
	     "sys time = %8.2f\n"
	     "mbits sec = %8.2f\n",
	     et.real_time, et.user_time, et.system_time, 
	     (total_bytes / et.real_time) * 8.0 / 1024.0 / 1024.0));
}

int
main (int argc, char **argv) 
{
  parse_args (argc,argv);

  // Pointer to the logger object that will be used.
  Logger logger (hostname, max_message_size);
  char	 *buf = new char [max_message_size];
  size_t total_bytes = 0;
  size_t nbytes = 0;

  logger.start_timer ();

  // Transmit logging records until user quits.

  for (int done = 0; done == 0;)
    switch (nbytes = transmit (logger, buf))
      {
      case -1:
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "transmit"), -1);
	/* NOTREACHED */
      case 0:
	done = 1;
	break;
      default:
	total_bytes += nbytes;
	break;
      }

  profile_logger::Elapsed_Time et;

  if (logger.stop_timer (et) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "stop timer"), -1);

  report_results (et, total_bytes);
  return 0;
}
