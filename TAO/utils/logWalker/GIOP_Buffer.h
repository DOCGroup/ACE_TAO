// -*- C++ -*-

//

#ifndef LOG_WALKER_GIOP_BUFFER_H
#define LOG_WALKER_GIOP_BUFFER_H

#include "ace/SString.h"
#include "ace/CDR_Stream.h"

class Invocation;
class Thread;

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

class GIOP_Buffer
{
public:
  static const char *size_leadin;
  static size_t leadin_len;
  static const size_t giop_header_len;
  static void init_leadin (int version);

  GIOP_Buffer (const char *text,
               size_t offset,
               Thread *thread,
               Invocation *owner = 0);
  GIOP_Buffer ();
  ~GIOP_Buffer ();

  void owner (Invocation *);
  Invocation *owner ();

  void init_buf (const char *text, size_t offset);
  int add_octets(const char *text, size_t offset);
  char type () const;
  char expected_type () const;
  bool sending () const;
  char minor_version () const;
  size_t reply_status () const;
  size_t num_contexts () const;
  bool is_oneway ();
  bool is_full () const;
  size_t log_posn () const;
  Thread *thread ();

  const ACE_Time_Value & time () const;
  void time (const ACE_Time_Value &);

  const ACE_CString &preamble() const;
  size_t expected_req_id() const;
  size_t actual_req_id();

  size_t expected_size () const;
  size_t buf_size () const;
  size_t cur_size() const;
  size_t msg_size ();

  const char * target_oid (size_t &len);
  const char * operation ();
  ACE_InputCDR &payload ();

  bool has_octets () const;
  bool validate ();
  bool matches (GIOP_Buffer *other) const;

  void reset ();
  void transfer_from (GIOP_Buffer *other);
  void swap (GIOP_Buffer *other);

private:
  bool parse_svc_contexts ();
  bool parse_header ();

  ACE_InputCDR *cdr_;
  ACE_CString preamble_;
  size_t log_offset_;
  Thread *thr_;
  ACE_Time_Value time_;
  size_t expected_req_id_;
  size_t expected_size_;
  char   expected_type_;
  size_t buffer_size_;
  char * wr_pos_;
  char * octets_;
  Invocation *owner_;
  bool buffer_lost_;
  bool sending_;
  char * oid_;
  size_t oid_len_;
  char * opname_;
  size_t req_id_;
  char   resp_exp_;
  size_t reply_status_;
  char   ver_minor_;
  size_t msg_size_;
  size_t  num_contexts_;
  bool header_parsed_;
  char * payload_start_;
  size_t payload_size_;
};

#endif // LOG_WALKER_GIOP_BUFFER_H
