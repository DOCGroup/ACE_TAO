/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#ifndef _PROCESS_PER_CONNECTION_LOGGING_SERVER_H
#define _PROCESS_PER_CONNECTION_LOGGING_SERVER_H

#include "ace/INET_Addr.h"
#include "ace/Log_Record.h"
#include "ace/Process.h"
#include "ace/Process_Manager.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Signal.h"
#include "Logging_Server.h"

class Logging_Process : public ACE_Process
{
public:
  Logging_Process (const char *prog_name,
                   const ACE_SOCK_Stream &logging_peer);

  virtual int prepare (ACE_Process_Options &options);
  virtual void unmanage ();

private:
  Logging_Process (); // Force desired constructor to be used.

  char prog_name_[MAXPATHLEN + 1];
  ACE_SOCK_Stream logging_peer_;
};


class Process_Per_Connection_Logging_Server : Logging_Server
{
protected:
  char prog_name_[MAXPATHLEN + 1];

protected:
  virtual int handle_connections ();
  virtual int handle_data (ACE_SOCK_Stream *client = 0);

  int run_master (int argc, char *argv[]);
  int run_worker (int argc, char *argv[]);

public:
  virtual int run (int argc, char *argv[]);

};

#endif /* _PROCESS_PER_CONNECTION_LOGGING_SERVER_H */
