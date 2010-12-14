// -*- C++ -*-

// $Id$
//

#ifndef LOG_WALKER_INVOCATION_H
#define LOG_WALKER_INVOCATION_H

#include "ace/SString.h"
#include "ace/CDR_Stream.h"

class PeerProcess;
class PeerObject;
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

class Invocation
{
public:
  class GIOP_Buffer
  {
  public:
    GIOP_Buffer (const char *text, size_t offset, Thread *thread, Invocation *owner = 0);
    ~GIOP_Buffer (void);
    void owner (Invocation *);
    Invocation *owner (void);
    void init_buf (const char *text);
    int add_octets(const char *text);
    char type (void) const;
    char expected_type (void) const;
    bool sending (void) const;
    char minor_version (void) const;
    char reply_status (void) const;
    size_t num_contexts (void) const;
    bool is_oneway (void);
    bool is_full (void) const;
    size_t log_posn (void) const;
    Thread *thread (void);
    time_t time (void) const;

    const ACE_CString &preamble(void) const;
    size_t expected_req_id(void) const;
    size_t actual_req_id(void);
    size_t expected_size (void) const;
    size_t size (void) const;
    size_t cur_size(void) const;
    const char * target_oid (size_t &len);
    const char * operation (void);
    bool validate (void);
    bool matches (GIOP_Buffer *other) const;
    void reset (void);
    void transfer_from (GIOP_Buffer *other);

  private:
    bool parse_svc_contexts (ACE_InputCDR& cdr);
    bool parse_header (void);

    ACE_CString preamble_;
    size_t log_offset_;
    Thread *thr_;
    time_t time_;
    size_t expected_req_id_;
    size_t expected_size_;
    char   expected_type_;
    size_t size_;
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
    size_t  num_contexts_;
    bool header_parsed_;
  };

  enum Dump_Mode {
    Dump_Proc,
    Dump_Thread,
    Dump_Both
  };

  // initialize a new instance, with the initial request text line and offeset
  Invocation (PeerProcess *peer, long handle, size_t req_id = 0);
  bool init ( const char * text, size_t offset, Thread *thr);
  ~Invocation (void);

  // return true if the invocation was a oneway
  bool is_oneway(void) const;

  void set_target (const char *oid, size_t oid_len);

  // return true if the request is a oneway and has all its octets, or
  // if it also has all its reply octets. The side-effect of this call
  // is to obtain the target reference from the request buffer and associate
  // it with the peer process.
  bool message_complete (void);

  size_t request_id (void) const;

  // returns the size parsed from either the request or reply preamble
  // which can be used to match a HEXDUMP b
  size_t expected_size (void) const;

  void set_octets (bool request, GIOP_Buffer *octets);
  GIOP_Buffer *octets (bool request);
  GIOP_Buffer *give_octets (bool request);

  bool contains (size_t line);
  size_t req_line (void);

  long handle (void) const;

  void dump_detail (ostream &strm, int indent, Dump_Mode mode, bool show_handle);

private:
  GIOP_Buffer *req_octets_;
  GIOP_Buffer *repl_octets_;

  PeerProcess *peer_;
  size_t req_id_;
  PeerObject *target_;
  long handle_;
};

#endif // LOG_WALKER_INVOCATION_H
