// -*- C++ -*-

// $Id$
// 

#ifndef LOG_WALKER_PEER_PROCESS_H
#define LOG_WALKER_PEER_PROCESS_H

#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers.h"
#include "ace/streams.h"

class PeerObject;
class HostProcess;
class Invocation;
class Session;

typedef ACE_Hash_Map_Manager<u_long, PeerObject*, ACE_Null_Mutex> PeerObjectTable;
typedef ACE_DLList<Invocation> InvocationList;

class PeerProcess
{
public:
  static char *nextIdent(void);
  PeerProcess (const char *text, 
               size_t offset, 
               bool as_client, 
               const char *target_addr, 
               long handle_);
  ~PeerProcess (void);
  void handle (long h);
  long handle (void) const;
  bool is_client (void) const;
  const char * id (void) const;

  void set_owner (HostProcess *host);
  HostProcess *owner (void);

  void match_hosts (Session *session);
  void set_addr (const char *addr, bool local);
  bool match_local (const char *addr) const;

  void close (size_t offset);
  bool is_closed (void) const;

  Invocation *new_invocation (size_t req_id);
  Invocation *find_invocation (size_t req_id);
  Invocation *find_invocation_size (size_t len);

  PeerObject *object_for(const char *oid, size_t len);

  void dump_summary (ostream &strm);
  void dump_object_detail (ostream &strm);
  void dump_invocation_detail (ostream &strm);

private:
  char *ident_;
  HostProcess *owner_;
  HostProcess *remote_;
  ACE_CString local_addr_;
  ACE_CString remote_addr_;
  long handle_;
  bool client_;
  ACE_CString origin_;
  size_t origin_offset_;
  size_t closed_offset_;
  PeerObjectTable objects_;
  InvocationList invocations_;
};

#endif // LOG_WALKER_PEER_PROCESS_H
