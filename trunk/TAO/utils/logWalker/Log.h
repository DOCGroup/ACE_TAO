// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_LOG_H
#define LOG_WALKER_LOG_H

#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "PeerProcess.h"
#include "HostProcess.h"

class Session;
class HostProcess;

typedef ACE_DLList<HostProcess> HostProcesses;

class Log
{
public:
  Log (Session &s);

  ~Log();

  bool init (const ACE_TCHAR *filename, const char *alias = "");

  void summarize(void);

  void dump (ostream &strm);

private:
  typedef void (* parser_func)(Log *this_, char *line, size_t offset);

  struct parse_key
  {
    const char *text;
    parser_func op;
  };

  bool get_pid_tid (long &pid, long &tid, char *line);
  HostProcess *get_host (long pid);
  void handle_msg_dump (char *line, size_t offset);
  void parse_line (char* line, size_t offset);

  static void parse_handler_open (Log *this_, char *line, size_t offset);
  static void parse_dump_msg (Log *this_, char *line, size_t offset);
  static void parse_HEXDUMP (Log *this_, char *line, size_t offset);
  static void parse_open_listener (Log *this_, char *line, size_t offset);
  static void parse_got_existing (Log *this_, char *line, size_t offset);
  static void parse_muxed_tms (Log *this_, char *line, size_t offset);
  static void parse_exclusive_tms (Log *this_, char *line, size_t offset);
  static void parse_process_parsed_msgs (Log *this_, char *line, size_t offset);
  static void parse_wait_for_event (Log *this_, char *line, size_t offset);
  static void parse_cleanup_queue (Log *this_, char *line, size_t offset);
  static void parse_close_connection (Log *this_, char *line, size_t offset);
  static void parse_begin_connection (Log *this_, char *line, size_t offset);
  static void parse_local_addr (Log *this_, char *line, size_t offset);
  static void parse_open_as_server (Log *this_, char *line, size_t offset);

  ACE_CString origin_;
  ACE_CString alias_;
  Session &session_;
  HostProcesses procs_;
  Invocation::GIOP_Buffer* dump_target_;
  ThreadList giop_waiters_;

};

#endif // LOG_WALKER_LOG_H
