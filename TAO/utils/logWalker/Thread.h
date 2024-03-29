// -*- C++ -*-

//

#ifndef LOG_WALKER_THREAD_H
#define LOG_WALKER_THREAD_H

#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"
#include "Invocation.h"
#include "PeerProcess.h"

typedef ACE_Unbounded_Stack<PeerProcess *> PeerProcessStack;
typedef ACE_Unbounded_Stack<Invocation *> InvocationStack;

class Thread
{
public:
  Thread (long tid, const char *alias, size_t offset);
  void add_time (const ACE_CString &time);

  long max_depth () const;
  long client_encounters () const;
  long server_encounters () const;
  long id () const;
  const ACE_CString &alias () const;
  void split_filename (char *buff, size_t len) const;

  void incoming_from (PeerProcess *);
  void add_invocation (Invocation *);
  void push_invocation (Invocation *);
  void pop_invocation ();
  Invocation *current_invocation () const;

  PeerProcess *incoming () const;
  void handle_request ();
  void enter_wait (PeerProcess *);
  void exit_wait (PeerProcess *, size_t linenum);
  GIOP_Buffer *giop_target ();
  void set_giop_target (GIOP_Buffer *buffer);
  void dump_summary (ostream &strm);
  void dump_invocations (ostream &strm);
  void dump_incidents (ostream &strm, const ACE_Time_Value& start);

  void get_summary (long &sent_reqs, long &recv_reqs, size_t &sent_size, size_t &recv_size);

  void push_new_connection (PeerProcess *pp);
  PeerProcess *pop_new_connection ();
  PeerProcess *peek_new_connection () const;

  void pending_local_addr (const ACE_CString &addr);
  const ACE_CString& pending_local_addr () const;
  void active_handle (long handle);
  long active_handle () const;
  void set_dup (Thread *other, bool set_other);
  void clear_dup ();
  void swap_target ();
  bool has_dup ();

  size_t count_nesting ();

private:
  long id_;
  ACE_CString alias_;
  size_t max_depth_;
  long client_encounters_;
  long server_encounters_;
  size_t nested_;
  PeerProcessStack pending_;
  PeerProcess *incoming_;
  PeerProcessStack new_connection_;
  ACE_CString pending_local_addr_;
  GIOP_Buffer *giop_target_;
  Thread *target_dup_;
  InvocationList invocations_;
  InvocationStack current_invocation_;
  long active_handle_;
  size_t first_line_;
  ACE_CString first_time_;
  ACE_CString last_time_;
};

#endif // LOG_WALKER_THREAD_H
