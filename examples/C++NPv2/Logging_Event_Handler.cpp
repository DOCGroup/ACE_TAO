/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/FILE_Connector.h"

#include "Logging_Event_Handler.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_netdb.h"

int Logging_Event_Handler::open () {

  static const char LOGFILE_SUFFIX[] = ".log";
  char filename[MAXHOSTNAMELEN + sizeof (LOGFILE_SUFFIX)];
  ACE_INET_Addr logging_peer_addr;

  logging_handler_.peer ().get_remote_addr (logging_peer_addr);
  logging_peer_addr.get_host_name (filename, MAXHOSTNAMELEN);
  ACE_OS::strcat (filename, LOGFILE_SUFFIX);

  ACE_FILE_Connector connector;
  connector.connect (log_file_,
                     ACE_FILE_Addr (filename),
                     0, // No timeout.
                     ACE_Addr::sap_any, // Ignored.
                     0, // Don't try to reuse the addr.
                     O_RDWR|O_CREAT|O_APPEND,
                     ACE_DEFAULT_FILE_PERMS);

  return reactor ()->register_handler
    (this, ACE_Event_Handler::READ_MASK);
}

int Logging_Event_Handler::handle_input (ACE_HANDLE)
{ return logging_handler_.log_record (); }

int Logging_Event_Handler::handle_close (ACE_HANDLE,
                                         ACE_Reactor_Mask) {
  logging_handler_.close ();
  log_file_.close ();
  delete this;
  return 0;
}
