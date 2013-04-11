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
class Endpoint;

typedef ACE_RB_Tree<long, HostProcess *, ACE_Less_Than<long>, ACE_Null_Mutex> Processes;
typedef ACE_RB_Tree<ACE_CString, HostProcess *, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex> Procs_By_Name;
typedef ACE_Hash_Map_Manager<const ACE_CString, ACE_CString, ACE_Null_Mutex> AltAddresses;

class Session
{
public:
  Session ();

  ~Session (void);

  void add_process (HostProcess *proc);
  HostProcess *find_process (long pid);
  HostProcess *find_host (const Endpoint &endpoint, bool server);
  void reconcile (void);

  static bool set_tao_version (ACE_TCHAR *str);
  static long tao_version (void);
  static void alternate_address (const char *string);
  static bool is_equivalent (const ACE_CString &primary,
                             const ACE_CString &alternate);
  void default_service (const char *string);

  void make_dir (const char * );
  void outfile (const char * );

  bool has_dir (void);
  bool has_outfile (void);

  void dump ();

private:
  HostProcess *find_host_i (const Endpoint &endpoint, bool server);

  ostream * stream_for ( ostream *, HostProcess * = 0, const char * = 0);

  Processes processes_;
  Procs_By_Name procs_by_name_;
  ACE_CString base_dir_;
  ACE_CString outfile_;
  static long tao_version_;
  static AltAddresses alt_addrs_;
};

#endif // LOG_WALKER_SESSION_H
