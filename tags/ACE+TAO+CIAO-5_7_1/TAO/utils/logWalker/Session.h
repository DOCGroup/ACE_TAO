// -*- C++ -*-

// $Id$
// 

#ifndef LOG_WALKER_SESSION_H
#define LOG_WALKER_SESSION_H

#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Synch.h"

class Log;
class HostProcess;

typedef ACE_Hash_Map_Manager<long, HostProcess *, ACE_Null_Mutex> Processes;
typedef ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_Null_Mutex> AltAddresses;

class Session
{
public:
  Session ();

  ~Session (void);

  void add_process (HostProcess *proc);
  HostProcess *find_process (long pid);
  HostProcess *find_host (const char *endpoint);
  void reconcile (void);

  static void set_tao_version (ACE_TCHAR *str);
  static long tao_version (void);
  void alternate_address (const char *string);
  void default_service (const char *string);

  void dump (ostream &strm) const;

private:
  Processes processes_;
  AltAddresses alt_addrs_;
  static long tao_version_;
};

#endif // LOG_WALKER_SESSION_H
