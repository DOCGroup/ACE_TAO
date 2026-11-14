#ifndef HTTPU_HTTP_BASE_H
#define HTTPU_HTTP_BASE_H

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "JAWS/Parse_Headers.h"
#include "HTTPU/http_export.h"
#include "HTTPU/http_status.h"
#include "HTTPU/http_headers.h"

class HTTPU_Export HTTP_Base : public HTTP_SCode_Base
{
public:
  HTTP_Base ();
  virtual ~HTTP_Base ();
  HTTP_Base (HTTP_Base const &) = delete;
  HTTP_Base (HTTP_Base &&) = delete;
  HTTP_Base &operator= (HTTP_Base const &) = delete;
  HTTP_Base &operator= (HTTP_Base &&) = delete;

  virtual int receive (ACE_Message_Block &mb);
  virtual int deliver (ACE_Message_Block &mb);

  virtual int receive_payload (ACE_Message_Block &mb);
  virtual int receive_payload (ACE_Message_Block &mb, long length);

  const char * payload ();
  unsigned long payload_size ();

  int status () const;
  const char *line () const;
  HTTP_Headers *http_headers ();
  JAWS_Headers *headers ();

  /// Replace matching headers or add new headers when no match exists.
  int build_headers (JAWS_Headers *new_headers);

  void dump ();

protected:
  /// Parse the initial line while receiving; this sets status_.
  virtual void parse_line () = 0;

  /// Create the initial line while delivering; return 0 on success, -1 on failure.
  virtual int espouse_line () = 0;

  /// Return the next delivery state.
  int deliver_header_name (JAWS_Header_Data *&data);

  /// Return the next delivery state.
  int deliver_header_value (JAWS_Header_Data *&data);

  /// Extract the first line of a request or response. Return 0 if more data is
  /// needed, 1 if the line was parsed successfully.
  virtual int extract_line (ACE_Message_Block &mb);

protected:
  int status_;
  /// Owns the buffer allocated by ACE_OS::strdup.
  char *line_;
  int deliver_state_;
  bool no_headers_;
  HTTP_Headers info_;
  JAWS_Header_Table_Iterator iter_;
  ACE_Message_Block_Unique_Ptr mb_ {};
  ACE_Message_Block payload_;
};

#if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "HTTPU/http_base.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#endif /* !defined (HTTPU_HTTP_BASE_H) */
