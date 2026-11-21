// -*- C++ -*-

//

#ifndef LOG_WALKER_INVOCATION_H
#define LOG_WALKER_INVOCATION_H

#include "ace/SString.h"
#include "ace/CDR_Stream.h"
#include <vector>

class PeerProcess;
class PeerObject;
class Thread;
class GIOP_Buffer;

using NotifyIncidents = std::vector<ACE_CString>;

/// Holds the buffer contents for a request/response pair. It may originate
/// in this process or in a peer process.
///
/// An outgoing invocation is detected when Muxed_TMS[%d] is followed by
/// the process or thread performing dump_msg. An incoming invocation is
/// detected when Transport[%d]::process_parsed_messages reports that no
/// data is missing. Depending on the connection context, this may indicate
/// a request or a reply.
///
/// Concurrent dump_msg calls can print the preamble and body out of order.
/// The hexdump size includes the 12-byte GIOP header. Its first line contains
/// header data that can be compared with the expected request ID.

class Invocation
{
public:
  enum Dump_Mode {
    Dump_Proc,
    Dump_Thread,
    Dump_Both
  };

  /// Initializes an instance with the initial request text line and offset.
  Invocation (PeerProcess *peer, Thread *thr, size_t req_id = 0);
  bool init ( const char * text, size_t offset, Thread *thr);
  ~Invocation ();

  /// Returns whether the invocation was a oneway call.
  bool is_oneway() const;

  /// Returns whether the request is a oneway and has all its octets, or
  /// whether it also has all its reply octets. This obtains the target
  /// reference from the request buffer and associates it with the peer process.
  bool message_complete ();

  size_t request_id () const;

  /// Returns whether this request was sent by the host process.
  bool sent_request () const;

  /// Returns the size parsed from either the request or reply preamble,
  /// which can be used to match a HEXDUMP buffer.
  size_t expected_size () const;

  size_t request_bytes () const;

  void set_target (const char *oid, size_t oid_len);

  void set_octets (bool request, GIOP_Buffer *octets);
  GIOP_Buffer *octets (bool request);
  GIOP_Buffer *give_octets (bool request);

  bool contains (size_t line);
  size_t req_line ();
  size_t repl_line ();

  void add_notify_incident (const ACE_CString &text, size_t offset);
  Thread *waiter () const;
  long handle () const;

  void new_line (ostream &strm, size_t indent, int offset,  bool add_nl, bool show_indent);
  void dump_detail (ostream &strm, size_t indent, Dump_Mode mode, bool show_handle);
  void dump_rel_time (ostream &strm, const ACE_Time_Value& tv, const ACE_Time_Value& start);
  void dump_start_line (ostream &strm, size_t indent, const ACE_Time_Value& start);
  void dump_finish_line (ostream &strm, size_t indent, const ACE_Time_Value& start);
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
