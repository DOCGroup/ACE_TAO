// This program sends logging records directly to the server, rather
// @(#)direct_logging.cpp	1.1	10/18/96

// than going through the client logging daemon.

#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"

static u_short LOGGER_PORT = ACE_DEFAULT_SERVER_PORT;
static const char *const LOGGER_HOST = ACE_DEFAULT_SERVER_HOST;
static const char *const DATA = "hello world\n";

int 
main (int argc, char *argv[])
{
  u_short logger_port  = argc > 1 ? atoi (argv[1]) : LOGGER_PORT;
  const char *logger_host = argc > 2 ? argv[2] : LOGGER_HOST;

  ACE_SOCK_Stream logger;
  ACE_SOCK_Connector connector;
  ACE_INET_Addr addr (logger_port, logger_host);
  ACE_Log_Record log_record (LM_DEBUG, 
			     ACE_OS::time ((time_t *) 0), 
			     ACE_OS::getpid ());

  if (connector.connect (logger, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  
  log_record.msg_data (DATA);
  size_t len = log_record.length ();
  size_t encoded_len = htonl (len);

  log_record.encode ();

  if (logger.send (4, &encoded_len, sizeof encoded_len,
		   (char *) &log_record, len) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
  else if (logger.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);
  return 0;
}
