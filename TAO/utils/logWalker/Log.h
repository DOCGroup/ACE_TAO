// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_LOG_H
#define LOG_WALKER_LOG_H

#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "ace/Time_Value.h"
#include "PeerProcess.h"
#include "HostProcess.h"
#include "GIOP_Buffer.h"

class Session;
class HostProcess;
class Invocation;

typedef ACE_DLList<HostProcess> HostProcesses;

class Incident
{
public:
  ACE_CString timestamp_;
  size_t offset_;
  int thread_;
  ACE_CString annotation_;
  Invocation *invocation_;
};

typedef ACE_Unbounded_Queue<Incident> IncidentList;


class Log
{
public:
  Log (Session &s);

  virtual ~Log(void);

  bool process_file (const ACE_TCHAR *filename, const char *alias = "");

protected:

  virtual void parse_line (void);

  void get_preamble (void);
  void get_timestamp (void);
  void handle_msg_octets (void);
  bool match_target(void);

  void parse_handler_open_i (bool is_ssl);
  void parse_connection_handler_ctor_i (void);
  void parse_dump_giop_msg_i (void);
  void parse_HEXDUMP_i (void);
  void parse_open_listener_i (void);
  void parse_got_existing_i (void);
  void parse_muxed_tms_i (void);
  void parse_exclusive_tms_i (void);
  void parse_process_parsed_msgs_i (void);
  void parse_wait_for_event_i (void);
  void parse_wait_on_read_i (void);
  void parse_cleanup_queue_i (void);
  void parse_complete_connection_i (void);
  void parse_close_connection_i (void);
  void parse_begin_connection_i (void);
  void parse_local_addr_i (void);
  void parse_connection_not_complete_i (void);
  void parse_open_as_server_i (void);
  void parse_iiop_connection_handler_ctor_i (void);
  void parse_wait_for_connection_i (void);
  void parse_post_open_i (void);
  void parse_notify_poa_helper_i (void);
  void parse_notify_object_i (void);

  ACE_CString origin_;
  ACE_CString alias_;
  Session &session_;
  HostProcesses procs_;
  GIOP_Buffer unknown_msg_;
  GIOP_Buffer* dump_target_;
  ThreadList giop_waiters_;
  ACE_DLList<PeerProcess> conn_waiters_;
  IncidentList history_;

  // parsed for every line
  ACE_CString timestamp_;
  char *line_;
  char *info_;
  size_t offset_;
  HostProcess *hostproc_;
  Thread *thr_;
};

#endif // LOG_WALKER_LOG_H
