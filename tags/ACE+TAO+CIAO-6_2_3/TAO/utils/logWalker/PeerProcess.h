// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_PEER_PROCESS_H
#define LOG_WALKER_PEER_PROCESS_H

#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/RB_Tree.h"
#include "ace/Functor.h"
#include "ace/Containers.h"
#include "ace/streams.h"

class PeerObject;
class HostProcess;
class Invocation;
class Session;
class Thread;


enum EndpointRole {
  ER_UNKNOWN,
  ER_CLIENT,
  ER_SERVER
};

class Endpoint
{
public:
  Endpoint (void);
  Endpoint (const Endpoint &other);
  Endpoint (const char *addr, EndpointRole role = ER_UNKNOWN);
  void assign (const char *addr, EndpointRole role = ER_UNKNOWN);
  Endpoint & operator = (const Endpoint &other);
  bool operator == (const Endpoint &other) const;
  bool operator < (const Endpoint &other) const;
  bool is_client (void) const;

  ACE_CString addr_;
  ACE_CString host_;
  ACE_CString port_;
  bool is_localhost_;
  EndpointRole role_;
};

class Transport
{
public:
  Transport (const char *addr, bool is_client, size_t offset);
  long handle_;
  Endpoint client_endpoint_;
  size_t open_offset_;
  size_t close_offset_;
};


typedef ACE_RB_Tree<u_long, PeerObject*, ACE_Less_Than<u_long>, ACE_Null_Mutex> PeerObjectTable;
typedef ACE_RB_Tree<long, PeerObject*, ACE_Less_Than<long>, ACE_Null_Mutex> ObjectByIndex;
typedef ACE_DLList<Invocation> InvocationList;
typedef ACE_DLList<Transport> TransportList;

class PeerProcess
{
public:
  static char *nextIdent(bool is_server);
  PeerProcess (size_t offset, bool is_server);

  virtual ~PeerProcess (void);

  const char * id (void) const;

  void set_owner (HostProcess *host);
  HostProcess *owner (void);

  void match_hosts (Session *session);
  void set_server_addr (const ACE_CString &addr);
  const Endpoint &server_addr (void) const;
  const Endpoint &last_client_addr (void) const;

  bool is_server (void) const;
  size_t offset (void) const;
  void ssl (bool is_ssl);
  void add_transport (Transport *t);
  Transport *last_transport (void);
  Transport *find_transport (long handle);

  bool match_local (const char *addr) const;
  bool match_server_addr (const Endpoint &addr) const;

  Invocation *new_invocation (size_t req_id, Thread *thr);
  Invocation *find_invocation (size_t req_id, long handle);
  Invocation *find_invocation_size (size_t len);

  PeerObject *object_for(const char *oid, size_t len);

  virtual void dump_summary (ostream &strm);
  virtual void dump_object_detail (ostream &strm);
  virtual void dump_invocation_detail (ostream &strm);

private:
  char *ident_;
  char *origin_;
  HostProcess *owner_;
  HostProcess *remote_;
  Endpoint server_ep_;
  TransportList transports_;
  Transport *last_transport_;
  bool is_server_role_;
  bool ssl_;
  size_t origin_offset_;
  PeerObjectTable objects_;
  InvocationList invocations_;
  ObjectByIndex object_by_index_;
};


#endif // LOG_WALKER_PEER_PROCESS_H
