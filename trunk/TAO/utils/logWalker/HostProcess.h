// -*- C++ -*-

// $Id$
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


/*
 * Peer node is an element of the connection list which associates peer
 * processes to the host process by handle. The peer node does not own
 * the reference to the peer process, and the specific peer process may
 * change over time.
 */
struct PeerNode
{
  PeerNode (long h, PeerProcess *p);
  long handle_;
  PeerProcess *peer_;
};

typedef ACE_DLList<Thread> ThreadList;
typedef ACE_DLList<PeerNode> PeerArray;
typedef ACE_DLList<ACE_CString> AddrList;
typedef ACE_RB_Tree<ACE_CString, PeerProcess *, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex> PeerProcs;

/*
 * HostProcess encapsulates the state related a specific process instance
 * described by a log file. There may be more than one HostProcess in a
 * log file.
 */

class HostProcess
{
public:
  HostProcess (const ACE_CString &src, long pid);
  ~HostProcess (void);

  // set/get the name of the process. This is only provided by
  // an alias supplied to the logWalker command line or manifest file.
  void proc_name (const ACE_CString &name);
  const ACE_CString &proc_name (void) const;

  // Returns a thread instance based on thread id. Will create an instance
  // as needed.
  Thread * find_thread (long tid);

  // Returns a thread that had previously worked with handle h. May return
  // a null pointer.
  Thread * find_thread_for_handle (long h);

  // Returns the process id.
  long pid (void) const;

  // returns true if the supplied endpoint has been visited before. This
  // may be either a listen endpoint or a client endpoint used to connect
  // to another peer.
  bool has_endpoint (ACE_CString& addr, bool listen);

  // adds a new endpoint to the list of listen endpoints. Client endpoints
  // are added as part of the process to add a new peer process
  void add_client_endpoint (ACE_CString& addr);

  // adds a new endpoint to the list of listen endpoints. Client endpoints
  // are added as part of the process to add a new peer process
  void add_listen_endpoint (ACE_CString& addr);

  // add a peer to the connection list. If the peer is not in the
  // by_addr_ table, it will be added there too.
  void add_peer (long handle, PeerProcess *peer);

  // locate a peer process by handle or address
  PeerProcess *find_peer (const ACE_CString& addr);
  PeerProcess *find_peer (long handle);

  void remove_peer (long handle);

  // remove a peer by handle, noting the line.
  void close_peer (long handle, size_t offset);

  // various output methods
  void dump_ident (ostream &strm, const char *extra);
  void dump_summary (ostream &strm);
  void dump_thread_detail (ostream &strm);
  void dump_thread_invocations (ostream &strm);
  void dump_peer_detail (ostream &strm);
  void dump_object_detail (ostream &strm);
  void dump_invocation_detail (ostream &strm);

  // reconcile_peers gives an opportunity to resolve peer process names
  // by matching the peer processes to other host processes in the session.
  void reconcile_peers (Session *session);

private:
  void iterate_peers (int group,
                      int operation,
                      ostream *strm = 0,
                      Session * session = 0);

  long pid_;
  ACE_CString logfile_name_;
  ACE_CString proc_name_;
  AddrList    client_endpoints_;
  AddrList    listen_endpoints_;
  ThreadList  threads_;

  PeerProcs   by_addr_;
  PeerArray   by_handle_;
};

#endif // LOG_WALKER_HOST_PROCESS_H
