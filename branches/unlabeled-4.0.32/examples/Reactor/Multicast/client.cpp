// client.C (written by Tim Harrison)
// @(#)client.cpp	1.1	10/18/96

// This program reads in messages from stdin and sends them
// to a Log_Wrapper.

#include "ace/Log_Msg.h"
#include "Log_Wrapper.h"

const char *MCAST_ADDR = ACE_DEFAULT_MULTICAST_ADDR;

// this is hardware specific.
// use netstat(1M) to find whether your interface
// is le0 or ie0

const int UDP_PORT = ACE_DEFAULT_MULTICAST_PORT;

// maximum message size
static int max_message_size = BUFSIZ * 20;

// number of times to send message of max_message_size
static int iterations = 0;

static void
parse_args (int argc, char *argv[])
{
  extern char *optarg;
  extern int  optind;
  int c;

  ACE_LOG_MSG->open (argv[0]);

  while ((c = ACE_OS::getopt (argc, argv, "m:ui:")) != -1)
    switch (c)
      {
      case 'm':
        max_message_size = ACE_OS::atoi (optarg) * BUFSIZ;
        break;
      case 'i':
	iterations = ACE_OS::atoi (optarg);
	break;
      case 'u':
	// usage fallthrough
      default:
        ACE_ERROR ((LM_ERROR, "%n: -m max_message_size (in k) -i iterations\n%a", 1));
	/* NOTREACHED */
      }
}

int
main (int argc, char **argv) 
{
  int user_prompt;

  parse_args (argc,argv);

  ACE_DEBUG ((LM_DEBUG, "Max Buffer size = %d\n", max_message_size));

  // Instantiate a log wrapper for logging
  Log_Wrapper log;

  // make a connection to a logger via orbixd
  if (log.open (UDP_PORT, MCAST_ADDR) == -1)
    ACE_OS::perror ("connect failed"), ACE_OS::exit (1);

  char *buf = new char[::max_message_size];

  // if -i has been specified, send max_message_size messages
  // iterations number of times
  if (iterations)
    {
      ACE_OS::memset (buf,1,::max_message_size);
      while (iterations--)
	if (log.log_message (Log_Wrapper::LM_DEBUG, buf) == -1)
	  perror("log failed."), exit(1);
    }

  // otherwise, a file has been redirected, or give prompts
  else
    {
      // If a file has been redirected, don't activate user prompts
      if (ACE_OS::isatty (0))
	user_prompt = 1;
      else
	user_prompt = 0;
      
      int nbytes;
      // continually read messages from stdin and log them.
      while (1) 
	{
	  if (user_prompt)
	    ACE_DEBUG ((LM_DEBUG, "\nEnter message ('Q':quit):\n"));
	  
	  if ((nbytes = read (0, buf, max_message_size)) == 0)
	    break; // end of file
	  buf[nbytes] = '\0';
	  
	  // quitting?
	  if (buf[0] == 'Q')
	    break;
	  
	  // send the message to the logger
	  else if (log.log_message (Log_Wrapper::LM_DEBUG, buf) == -1)
	    perror("log failed."), exit(1);
	} // while(1)
    }

  ACE_DEBUG ((LM_DEBUG, "Client done.\n"));
  return 0;
}
