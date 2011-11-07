// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_THREAD_H
#define LOG_WALKER_THREAD_H

#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"
#include "Invocation.h"
#include "PeerProcess.h"

typedef ACE_Unbounded_Stack<PeerProcess *> UpcallStack;

class Thread
{
public:
  Thread (long tid, const char *alias);
  long max_depth (void) const;
  long encounters (void) const;
  long id (void) const;
  const ACE_CString &alias (void) const;
  void incoming_from (PeerProcess *);
  void add_invocation (Invocation *);

  PeerProcess *incoming (void) const;
  void handle_request (void);
  void enter_wait (PeerProcess *);
  void exit_wait (PeerProcess *, size_t linenum);
  Invocation::GIOP_Buffer *giop_target (void);
  void set_giop_target (Invocation::GIOP_Buffer *buffer);
  void dump_detail (ostream &strm);
  void dump_invocations (ostream &strm);
  PeerProcess *pending_peer (void) const;
  void pending_peer (PeerProcess *pp);

  void active_handle (long handle);
  long active_handle (void) const;

private:
  long id_;
  ACE_CString alias_;
  size_t max_depth_;
  long encounters_;
  long nested_;
  UpcallStack pending_;
  PeerProcess *incoming_;
  PeerProcess *new_connection_;
  Invocation::GIOP_Buffer *giop_target_;
  InvocationList invocations_;
  long active_handle_;
};

#endif // LOG_WALKER_THREAD_H
