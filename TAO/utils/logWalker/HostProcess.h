// -*- C++ -*-

// $Id$
// 

#ifndef LOG_WALKER_HOST_PROCESS_H
#define LOG_WALKER_HOST_PROCESS_H
#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"
#include "Invocation.h"
#include "PeerProcess.h"

class Session;
class Thread;

typedef ACE_DLList<Thread> ThreadList;
typedef ACE_DLList<ACE_CString> AddrList;
typedef ACE_Hash_Map_Manager<long, PeerProcess *, ACE_Null_Mutex> PeerProcs;

class HostProcess
{
public:
  HostProcess (const ACE_CString &src, long pid);
  ~HostProcess (void);
  void proc_name (const ACE_CString &name);
  const ACE_CString &proc_name (void) const;
  Thread * find_thread (long tid);
  long pid (void) const;
  bool has_endpoint (ACE_CString& addr);
  void add_endpoint (ACE_CString& addr);
  void add_peer (PeerProcess *peer, bool as_client);
  PeerProcess *find_peer (long handle);
  void close_peer (long handle, size_t offset);

  void dump_ident (ostream &strm, const char *extra);
  void dump_summary (ostream &strm);
  void dump_thread_detail (ostream &strm);
  void dump_thread_invocations (ostream &strm);
  void dump_peer_detail (ostream &strm);
  void dump_object_detail (ostream &strm);
  void dump_invocation_detail (ostream &strm);

  void reconcile_peers (Session *session);

private:
  void dump_detail_list (ostream &strm, PeerProcs &list, int mode, Session * session = 0);

  long pid_;
  ACE_CString logfile_name_;
  ACE_CString proc_name_;
  AddrList   endpoints_;
  ThreadList threads_;
  PeerProcs  clients_;
  PeerProcs  servers_;
  //  PeerProcs  closed_;
  long closed_handles_;
};

#endif // LOG_WALKER_HOST_PROCESS_H
