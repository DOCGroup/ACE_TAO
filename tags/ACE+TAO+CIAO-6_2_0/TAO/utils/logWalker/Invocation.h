// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_INVOCATION_H
#define LOG_WALKER_INVOCATION_H

#include "ace/SString.h"
#include "ace/CDR_Stream.h"
#include "ace/Unbounded_Queue.h"

class PeerProcess;
class PeerObject;
class Thread;
class GIOP_Buffer;

typedef ACE_Unbounded_Queue<ACE_CString> NotifyIncidents;

// Invocation holds the buffer contents for a request/response pair.
// This could be originating in this process, or in the peer process.
//
// The trigger for finding a new outgoing invocation is "Muxed_TMS[%d]"
// following that the process/thread will perform a dump_msg.
//
// The trigger for finding a new incoming invocation is
// "Transport[%d]::process_parsed_messages, entering (missing data == 0)"
// which could indicate a new request or reply, depending on the context
// in which the peer connection is made.
//
// It is possible that two or more threads may call dump_msg
// concurrently and thus have the preamble and body printed out of
// order. The HEXDUMP always reports a buffer size including the 12
// byte GIOP header. Also, the first line of the text contains header
// data which can be compared to the expected request ID.

class Invocation
{
public:
  enum Dump_Mode {
    Dump_Proc,
    Dump_Thread,
    Dump_Both
  };

  // initialize a new instance, with the initial request text line and offeset
  Invocation (PeerProcess *peer, Thread *thr, size_t req_id = 0);
  bool init ( const char * text, size_t offset, Thread *thr);
  ~Invocation (void);

  // return true if the invocation was a oneway
  bool is_oneway(void) const;

  // return true if the request is a oneway and has all its octets, or
  // if it also has all its reply octets. The side-effect of this call
  // is to obtain the target reference from the request buffer and associate
  // it with the peer process.
  bool message_complete (void);

  size_t request_id (void) const;

  // returns true if this request was sent by the host process.
  bool sent_request (void) const;

  // returns the size parsed from either the request or reply preamble
  // which can be used to match a HEXDUMP b
  size_t expected_size (void) const;

  size_t request_bytes (void) const;

  void set_target (const char *oid, size_t oid_len);

  void set_octets (bool request, GIOP_Buffer *octets);
  GIOP_Buffer *octets (bool request);
  GIOP_Buffer *give_octets (bool request);

  bool contains (size_t line);
  size_t req_line (void);
  size_t repl_line (void);

  void add_notify_incident (const ACE_CString &text, size_t offset);
  Thread *waiter (void) const;
  long handle (void) const;

  void new_line (ostream &strm, int indent, int offset,  bool add_nl, bool show_indent);
  void dump_detail (ostream &strm, size_t indent, Dump_Mode mode, bool show_handle);
  void dump_start_line (ostream &strm, size_t indent);
  void dump_finish_line (ostream &strm, size_t indent);
  void dump_special_details (ostream &strm, size_t indent, const char *opname);

private:
  GIOP_Buffer *req_octets_;
  GIOP_Buffer *repl_octets_;

  Thread *waiter_;
  NotifyIncidents notify_incidents_;
  PeerProcess *peer_;
  size_t req_id_;
  PeerObject *target_;
  long handle_;
  bool finish_reported_;
  size_t req_level_;
  size_t repl_level_;
};

#endif // LOG_WALKER_INVOCATION_H
