// $Id$

#include "ace/INet/FTP_IOStream.h"
#include "ace/INet/IOS_util.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/FTP_IOStream.inl"
#endif

#include "ace/Truncate.h"



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {

    StreamBuffer::StreamBuffer (std::iostream * stream)
      : ACE::IOS::BufferedStreamBuffer (BUFFER_SIZE,
                                        std::ios::in | std::ios::out),
        stream_ (stream)
      {
      }

    StreamBuffer::~StreamBuffer ()
      {
      }

    std::iostream* StreamBuffer::set_stream (std::iostream * stream)
      {
        std::iostream * old_stream = this->stream_;
        this->stream_ = stream;
        this->reset_buffers ();
        return old_stream;
      }

    int StreamBuffer::read_from_stream (char* buffer, std::streamsize length)
      {
        if (this->stream_ == 0) return -1;
        this->stream_->read (buffer, length);
        return ACE_Utils::truncate_cast<int> (this->stream_->gcount ());
      }

    int StreamBuffer::write_to_stream (const char* buffer, std::streamsize length)
      {
        if (this->stream_ == 0) return -1;
        this->stream_->write (buffer, length);
        return this->stream_->good () ? ACE_Utils::truncate_cast<int> (length) : -1;
      }

    int StreamBuffer::sync ()
      {
        if (this->stream_ == 0) return -1;
        if (ACE::IOS::BufferedStreamBuffer::sync () == -1)
          return -1;
        return this->stream_->sync ();
      }

    IOS::IOS (std::iostream * stream)
      : streambuf_ (stream)
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

    std::iostream* IOS::set_stream (std::iostream* stream)
      {
        std::iostream* old_stream = this->rdbuf ()->set_stream (stream);
        this->clear ();
        return old_stream;
      }

    OStream::OStream(std::iostream * stream)
      : IOS (stream), std::ostream (&streambuf_)
      {
      }

    OStream::~OStream()
      {
      }

    IStream::IStream(std::iostream * stream)
      : IOS (stream), std::istream (&streambuf_)
      {
      }

    IStream::~IStream()
      {
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
