// This program sends logging records directly to the server, rather
// $Id$

// than going through the client logging daemon.

#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"

static u_short LOGGER_PORT = ACE_DEFAULT_SERVER_PORT;
static const char *const LOGGER_HOST = ACE_DEFAULT_SERVER_HOST;
static const int MAX_ITERATIONS = 10;

int 
main (int argc, char *argv[])
{
  const char *logger_host = argc > 1 ? argv[1] : LOGGER_HOST;
  u_short logger_port  = argc > 2 ? ACE_OS::atoi (argv[2]) : LOGGER_PORT;
  int max_iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : MAX_ITERATIONS;

  ACE_SOCK_Stream logger;
  ACE_SOCK_Connector connector;
  ACE_INET_Addr addr (logger_port, logger_host);

  if (connector.connect (logger, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  
  for (int i = 0; i < max_iterations; i++)
    {
      ACE_Log_Record log_record (LM_DEBUG, 
				 ACE_OS::time ((time_t *) 0), 
				 ACE_OS::getpid ());

      char buf[BUFSIZ];
      ::sprintf (buf, "message = %d\n", i + 1);
      log_record.msg_data (buf);
      size_t len = log_record.length ();
      size_t encoded_len = htonl (len);

      log_record.encode ();

      if (logger.send (4, &encoded_len, sizeof encoded_len,
		       (char *) &log_record, len) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
      else
	ACE_OS::sleep (1);
    }

  if (logger.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);

  return 0;
}
