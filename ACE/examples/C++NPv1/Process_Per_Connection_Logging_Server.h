/*
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
#include <string>

class Logging_Process : public ACE_Process
{
public:
  Logging_Process (char const *prog_name,
                   ACE_SOCK_Stream const &logging_peer);
  Logging_Process (Logging_Process const &) = delete;
  Logging_Process (Logging_Process &&) = delete;
  Logging_Process &operator= (Logging_Process const &) = delete;
  Logging_Process &operator= (Logging_Process &&) = delete;

  int prepare (ACE_Process_Options &options) override;
  void unmanage () override;

private:
  Logging_Process () = delete;

  std::string prog_name_;
  ACE_SOCK_Stream logging_peer_;
};


class Process_Per_Connection_Logging_Server : public Logging_Server
{
private:
  std::string prog_name_;

protected:
  int handle_connections () override;
  int handle_data (ACE_SOCK_Stream *client = nullptr) override;

  int run_master (int argc, char *argv[]);
  int run_worker (int argc, char *argv[]);

public:
  Process_Per_Connection_Logging_Server () = default;
  Process_Per_Connection_Logging_Server (Process_Per_Connection_Logging_Server const &) = delete;
  Process_Per_Connection_Logging_Server (Process_Per_Connection_Logging_Server &&) = delete;
  Process_Per_Connection_Logging_Server &operator= (Process_Per_Connection_Logging_Server const &) = delete;
  Process_Per_Connection_Logging_Server &operator= (Process_Per_Connection_Logging_Server &&) = delete;

  int run (int argc, char *argv[]) override;
};

#endif /* _PROCESS_PER_CONNECTION_LOGGING_SERVER_H */
