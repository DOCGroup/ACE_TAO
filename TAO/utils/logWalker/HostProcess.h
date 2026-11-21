// -*- C++ -*-

//

#ifndef LOG_WALKER_HOST_PROCESS_H
#define LOG_WALKER_HOST_PROCESS_H
#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/RB_Tree.h"
#include "ace/Functor.h"
#include "Invocation.h"
#include "PeerProcess.h"

class Session;
class Thread;

/// A peer node associates a peer process with the host process by handle.
/// It does not own the peer process reference, and the referenced process
/// may change over time.
struct PeerNode
{
  PeerNode (long h, PeerProcess *p);
  long handle_;
  PeerProcess *peer_;
};

using ThreadList = ACE_DLList<Thread>;
using PeerArray = ACE_DLList<PeerNode>;
using AddrList = ACE_DLList<Endpoint>;
using PeerProcs = ACE_RB_Tree<Endpoint, PeerProcess *, ACE_Less_Than<Endpoint>, ACE_Null_Mutex>;

/// Encapsulates the state of a process instance described by a log file.
/// A log file may describe more than one process.

class HostProcess
{
public:
  HostProcess (const ACE_CString &src, long pid);
  ~HostProcess ();

  /// Sets or returns the process name. This is only provided by an alias
  /// supplied to the logWalker command line or manifest file.
  void proc_name (const ACE_CString &name);
  const ACE_CString &proc_name () const;

  /// Returns the thread for an ID, creating it if needed.
  Thread * find_thread (long tid, size_t offset);

  /// Returns a thread that has a pending peer with the supplied address.
  Thread * find_thread_for_peer (const ACE_CString& addr);

  /// Returns a thread that previously worked with handle h, or null.
  Thread * find_thread_for_handle (long h);

  /// Returns the process ID.
  long pid () const;

  const ACE_Time_Value &start_time () const;
  void start_time (const ACE_Time_Value &);

  /// Returns whether the endpoint has been visited before. It may be a
  /// listen endpoint or a client endpoint used to connect to another peer.
  bool has_endpoint (const Endpoint& addr, bool listen);

  /// Adds a client endpoint to the endpoint list.
  void add_client_endpoint (const Endpoint& addr);

  /// Adds a listen endpoint to the endpoint list.
  void add_listen_endpoint (const Endpoint& addr);

  /// Adds a peer to the connection list and, if needed, the by_addr_ table.
  void add_peer (long handle, PeerProcess *peer);

  /// Finds a peer process by handle or address.
  PeerProcess *find_peer (const ACE_CString& addr);
  PeerProcess *find_peer (long handle);

  void remove_peer (long handle);

  /// Closes a peer by handle and records the line offset.
  void close_peer (long handle, size_t offset);

  /// Various output methods.
  void dump_ident (ostream &strm, const char *extra);
  void dump_summary (ostream &strm);
  void dump_thread_summary (ostream &strm);
  void split_thread_invocations (Session *session, const ACE_Time_Value& start);
  void dump_thread_invocations (ostream &strm, const ACE_Time_Value& start);
  void dump_peer_summary (ostream &strm);
  void dump_object_detail (ostream &strm);
  void split_peer_invocations (Session *session);
  void dump_invocation_detail (ostream &strm);

  /// Resolves peer process names by matching them to other host processes
  /// in the session.
  void reconcile_peers (Session *session);

private:
  void iterate_peers (int group,
                      int operation,
                      ostream *strm = nullptr,
                      Session *session = nullptr);

  long pid_;
  ACE_CString logfile_name_;
  ACE_CString proc_name_;
  AddrList    client_endpoints_;
  AddrList    listen_endpoints_;
  ThreadList  threads_;

  PeerProcs   by_addr_;
  PeerArray   by_handle_;
  ACE_Time_Value start_time_;
};

#endif // LOG_WALKER_HOST_PROCESS_H
