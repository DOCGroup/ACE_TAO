// This program reads in messages from stdin and sends them to a
// Log_Wrapper.

#include "ace/OS_main.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_Memory.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "Log_Wrapper.h"

#include <cstring>
#include <memory>

// Multi-cast address.
static constexpr char MCAST_ADDR[] = ACE_DEFAULT_MULTICAST_ADDR;

// UDP port.
static constexpr int UDP_PORT = ACE_DEFAULT_MULTICAST_PORT;

// Maximum message size.
static int max_message_size {BUFSIZ};

// Number of times to send message of max_message_size.
static int iterations {};

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  //FUZZ: disable check_for_lack_ACE_OS
  // Start at argv[1]
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT("m:ui:"), 1);

  for (int c; (c = getopt ()) != -1; )
  //FUZZ: enable check_for_lack_ACE_OS
    switch (c)
      {
      case 'm':
        max_message_size = ACE_OS::atoi (getopt.opt_arg ()) * BUFSIZ;
        break;
      case 'i':
        iterations = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'u':
        // usage same as unknown.
        ACE_FALLTHROUGH;
      default:
        ACE_ERROR ((LM_ERROR,
                    "%n: -m max_message_size (in k) -i iterations\n%a",
                    1));
        /* NOTREACHED */
      }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  bool user_prompt {};

  parse_args (argc,argv);

  ACE_DEBUG ((LM_DEBUG, "max buffer size = %d\n", max_message_size));

  // Instantiate a log wrapper for logging
  Log_Wrapper log;

  // Make a connection to a logger.
  if (log.open (UDP_PORT, MCAST_ADDR) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n" "open"), -1);

  char *raw_buffer {};
  ACE_NEW_RETURN (raw_buffer, char[max_message_size], -1);
  std::unique_ptr<char[]> buffer (raw_buffer);
  char *const buf = buffer.get ();

  // If -i has been specified, send max_message_size messages
  // iterations number of times.
  if (iterations)
    {
      std::memset (buf, 1, max_message_size);

      while (iterations--)
        if (log.log_message (Log_Wrapper::LM_DEBUG, buf) == -1)
          ACE_ERROR_RETURN ((LM_ERROR, "%p\n" "log"), -1);
    }

  // otherwise, a file has been redirected, or give prompts
  else
    {
      // If a file has been redirected, don't activate user prompts.
      if (ACE_OS::isatty (0))
        user_prompt = true;
      else
        user_prompt = false;

      // Continually read messages from stdin and log them.

      for (int count = 1;;)
        {
          if (user_prompt)
            ACE_DEBUG ((LM_DEBUG, "\nEnter message ('Q':quit):\n"));

          ssize_t const nbytes = ACE_OS::read (ACE_STDIN, buf, max_message_size);

          if (nbytes <= 0)
            break; // End of file or error.

          buf[nbytes - 1] = '\0';

          // Quitting?
          if (user_prompt)
            {
              if (buf[0] == 'Q' || buf[0] == 'q')
                break;
            }
          else // Keep from overrunning the receiver.
            ACE_OS::sleep (1);

          // Send the message to the logger.
          if (log.log_message (Log_Wrapper::LM_DEBUG, buf) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n" "log_message"), -1);

          ACE_DEBUG ((LM_DEBUG, "finished sending message %d\n", count++));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "Client done.\n"));
  return 0;
}
