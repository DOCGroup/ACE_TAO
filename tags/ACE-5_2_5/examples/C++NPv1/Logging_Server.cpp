/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "Logging_Server.h"

int Logging_Server::run (int argc, char *argv[])
{
  if (open (argc > 1 ? atoi (argv[1]) : 0) == -1)
    return -1;

  for (;;) {
    if (wait_for_multiple_events () == -1)
      return -1;
    if (handle_connections () == -1)
      return -1;
    if (handle_data () == -1)
      return -1;
  }

  return 0;
}


int Logging_Server::open (u_short logger_port)
{
  ACE_INET_Addr server_addr;
  int result;

  if (logger_port != 0)
    result = server_addr.set (logger_port, INADDR_ANY);
  else
    result = server_addr.set ("ace_logger", INADDR_ANY);
  if (result == -1) return -1;

  // Start listening, enable reuse of listen address for quick restarts.
  return acceptor_.open (server_addr, 1);
}


int Logging_Server::make_log_file (ACE_FILE_IO &logging_file,
                                   ACE_SOCK_Stream *logging_peer)
{
  char filename[MAXHOSTNAMELEN + sizeof (".log")];

  if (logging_peer != 0) { // Use client's hostname as log file name.
    ACE_INET_Addr logging_peer_addr;
    logging_peer->get_remote_addr (logging_peer_addr);
    logging_peer_addr.get_host_name (filename, MAXHOSTNAMELEN);
    strcat (filename, ".log");
  }
  else
    strcpy (filename, "logging_server.log");

  ACE_FILE_Connector connector;
  return connector.connect (logging_file,
                            ACE_FILE_Addr (filename),
                            0, // No timeout.
                            ACE_Addr::sap_any, // Ignored.
                            0, // Don't try to reuse the addr.
                            O_RDWR|O_CREAT|O_APPEND,
                            ACE_DEFAULT_FILE_PERMS);
}
