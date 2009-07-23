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
typedef ACE_DLList<PeerProcess> PeerProcesses;


class Log
{
public:
  Log (Session &s);

  ~Log();

  bool init (const ACE_TCHAR *filename, const char *alias = "");
  void parse_line (char* line, size_t offset);

  void summarize(void);

  void dump (ostream &strm);

private:
  bool get_pid_tid (long &pid, long &tid, char *line);
  HostProcess *get_host (long pid);

  ACE_CString origin_;
  ACE_CString alias_;
  Session &session_;
  HostProcesses procs_;
//  PeerProcesses peers_;
  Invocation::GIOP_Buffer* dump_target_;
  ThreadList giop_waiters_;

};

#endif // LOG_WALKER_LOG_H
