// $Id$

#ifndef ACE_HTTP_STREAM_POLICY_BASE_CPP
#define ACE_HTTP_STREAM_POLICY_BASE_CPP

#include "ace/INet/HTTP_StreamPolicyBase.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    template <class STREAM_BUFFER>
    StreamPolicyBase<STREAM_BUFFER>::StreamPolicyBase ()
      : streambuf_ (0)
      {}

    template <class STREAM_BUFFER>
    StreamPolicyBase<STREAM_BUFFER>::~StreamPolicyBase ()
      {}

    template <class STREAM_BUFFER>
    int StreamPolicyBase<STREAM_BUFFER>::read_from_stream_i (
        char_type * buf,
        std::streamsize length)
      {
        return this->streambuf_->read_from_stream_i (buf, length);
      }

    template <class STREAM_BUFFER>
    int StreamPolicyBase<STREAM_BUFFER>::write_to_stream_i (
        const char_type * buf,
        std::streamsize length)
      {
        return this->streambuf_->write_to_stream_i (buf, length);
      }

    template <class STREAM_BUFFER>
    void StreamPolicyBase<STREAM_BUFFER>::set_stream_buffer (STREAM_BUFFER* streambuf)
      {
        this->streambuf_ = streambuf;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HTTP_STREAM_POLICY_BASE_CPP */
