// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_SESSION_H
#define LOG_WALKER_SESSION_H

#include "ace/Hash_Map_Manager.h"
#include "ace/RB_Tree.h"
#include "ace/Functor.h"
#include "ace/SString.h"
#include "ace/Synch.h"

class Log;
class HostProcess;

typedef ACE_RB_Tree<long, HostProcess *, ACE_Less_Than<long>, ACE_Null_Mutex> Processes;
typedef ACE_RB_Tree<ACE_CString, HostProcess *, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex> Procs_By_Name;
typedef ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_Null_Mutex> AltAddresses;

class Session
{
public:
  Session ();

  ~Session (void);

  void add_process (HostProcess *proc);
  HostProcess *find_process (long pid);
  HostProcess *find_host (ACE_CString &endpoint, bool server);
  void reconcile (void);

  static bool set_tao_version (ACE_TCHAR *str);
  static long tao_version (void);
  static int date_format (void);
  static bool set_date_format (ACE_TCHAR *ser);

  void alternate_address (const char *string);
  void default_service (const char *string);

  void make_dir (const char * );
  void outfile (const char * );

  bool has_dir (void);
  bool has_outfile (void);

  void dump ();

private:
  ostream * stream_for ( ostream *, HostProcess * = 0, const char * = 0);

  Processes processes_;
  Procs_By_Name procs_by_name_;
  AltAddresses alt_addrs_;
  ACE_CString base_dir_;
  ACE_CString outfile_;
  static long tao_version_;
  static int date_format_;
};

#endif // LOG_WALKER_SESSION_H
