// $Id$

#include "ace/INet/HTTP_IOStream.h"
#include "ace/INet/IOS_util.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_IOStream.inl"
#endif

#include "ace/Truncate.h"



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    StreamBuffer::StreamBuffer (std::iostream& stream, StreamBuffer::policy_type* policy)
      : ACE::IOS::BufferedStreamBuffer (BUFFER_SIZE,
                                        std::ios::in | std::ios::out),
        stream_ (stream),
        policy_ (policy)
      {
        if (this->policy_)
          this->policy_->set_stream_buffer (this);
      }

    StreamBuffer::~StreamBuffer ()
      {
        if (this->policy_)
          delete this->policy_;
      }

    int StreamBuffer::read_from_stream (char* buffer, std::streamsize length)
      {
        if (this->policy_)
          return this->policy_->read_from_stream (buffer, length);
        else
          return this->read_from_stream_i (buffer, length);
      }

    int StreamBuffer::read_from_stream_i (char* buffer, std::streamsize length)
      {
        this->stream_.read (buffer, length);
        return ACE_Utils::truncate_cast<int> (this->stream_.gcount ());
      }

    int StreamBuffer::write_to_stream (const char* buffer, std::streamsize length)
      {
        if (this->policy_)
          return this->policy_->write_to_stream (buffer, length);
        else
          return this->write_to_stream_i (buffer, length);
      }

    int StreamBuffer::write_to_stream_i (const char* buffer, std::streamsize length)
      {
        this->stream_.write (buffer, length);
        return this->stream_.good () ? ACE_Utils::truncate_cast<int> (length) : -1;
      }

    int StreamBuffer::sync ()
      {
        if (ACE::IOS::BufferedStreamBuffer::sync () == -1)
          return -1;
        return this->stream_.sync ();
      }

    IOS::IOS (std::iostream& stream, StreamBuffer::policy_type* policy)
      : streambuf_ (stream, policy)
      {
        ace_ios_init (&this->streambuf_);
      }

    IOS::~IOS ()
      {
        try
          {
            this->streambuf_.sync();
          }
        catch (...)
          {
          }
      }

    OStream::OStream(std::iostream& stream, StreamBuffer::policy_type* policy)
      : IOS (stream, policy), std::ostream (&streambuf_)
      {
      }

    OStream::~OStream()
      {
      }

    IStream::IStream(std::iostream& stream, StreamBuffer::policy_type* policy)
      : IOS (stream, policy), std::istream (&streambuf_)
      {
      }

    IStream::~IStream()
      {
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
