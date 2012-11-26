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
typedef ACE_Unbounded_Stack<Invocation *> InvocationStack;

class Thread
{
public:
  Thread (long tid, const char *alias, size_t offset);
  long max_depth (void) const;
  long client_encounters (void) const;
  long server_encounters (void) const;
  long id (void) const;
  const ACE_CString &alias (void) const;
  void incoming_from (PeerProcess *);
  void add_invocation (Invocation *);
  void push_invocation (Invocation *);
  void pop_invocation (void);
  Invocation *current_invocation (void) const;

  PeerProcess *incoming (void) const;
  void handle_request (void);
  void enter_wait (PeerProcess *);
  void exit_wait (PeerProcess *, size_t linenum);
  GIOP_Buffer *giop_target (void);
  void set_giop_target (GIOP_Buffer *buffer);
  void dump_detail (ostream &strm) const;
  void dump_invocations (ostream &strm);
  void get_summary (long &sent_reqs, long &recv_reqs, size_t &sent_size, size_t &recv_size);
  PeerProcess *pending_peer (void) const;
  void pending_peer (PeerProcess *pp);
  void pending_local_addr (const ACE_CString &addr);
  const ACE_CString& pending_local_addr (void) const;
  void active_handle (long handle);
  long active_handle (void) const;
  void set_dup (Thread *other, bool set_other);
  void clear_dup (void);
  void swap_target (void);
  bool has_dup (void);

private:
  long id_;
  ACE_CString alias_;
  size_t max_depth_;
  long client_encounters_;
  long server_encounters_;
  long nested_;
  UpcallStack pending_;
  PeerProcess *incoming_;
  PeerProcess *new_connection_;
  ACE_CString pending_local_addr_;
  GIOP_Buffer *giop_target_;
  Thread *target_dup_;
  InvocationList invocations_;
  InvocationStack current_invocation_;
  long active_handle_;
  size_t first_line_;
};

#endif // LOG_WALKER_THREAD_H
