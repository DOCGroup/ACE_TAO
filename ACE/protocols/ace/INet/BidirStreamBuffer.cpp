// $Id$

#ifndef ACE_IOS_BIDIR_STREAM_BUFFER_CPP
#define ACE_IOS_BIDIR_STREAM_BUFFER_CPP

#include "ace/INet/BidirStreamBuffer.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::BasicBidirStreamBuffer (
        STREAM_HANDLER* sh,
        std::streamsize bufsz,
        openmode mode)
      : bufsize_ (bufsz),
        mode_ (mode),
        stream_ (sh),
        interceptor_ (0)
      {
        this->stream_->add_reference ();

        char_type* p = 0;
        ACE_NEW_NORETURN (p, char_type [bufsz]);
        this->read_buffer_.reset (p);
        p = 0;
        ACE_NEW_NORETURN (p, char_type [bufsz]);
        this->write_buffer_.reset (p);

        this->reset_buffers ();
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::~BasicBidirStreamBuffer ()
      {
        this->close_stream ();
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    typename BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::int_type
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::overflow (int_type c)
      {
        if (!(this->mode_ & ios_type::out)) return char_traits::eof ();

        if (c != char_traits::eof ())
        {
          *this->pptr () = char_traits::to_char_type (c);
          this->pbump (1);
        }
        if (this->flush_buffer () == std::streamsize (-1)) return char_traits::eof ();

        return c;
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    typename BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::int_type
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::underflow ()
      {
        if (!(this->mode_ & ios_type::in)) return char_traits::eof ();

        if (this->gptr () && (this->gptr () < this->egptr ()))
          return char_traits::to_int_type (*this->gptr ());

        int putback = int (this->gptr () - this->eback ());
        if (putback > 4) putback = 4;

        ACE_OS::memmove (this->read_buffer_.get () + (4 - putback),
                         this->gptr () - putback,
                         putback * sizeof (char_type));

        if (this->interceptor_)
          this->interceptor_->before_read (this->bufsize_ - 4);

        int n = this->read_from_stream (this->read_buffer_.get () + 4,
                                        this->bufsize_ - 4);

        if (this->interceptor_)
          this->interceptor_->after_read (this->read_buffer_.get () + 4, n);

        if (n <= 0)
          {
            if (this->interceptor_)
              this->interceptor_->on_eof ();

            return char_traits::eof ();
          }

        this->setg (this->read_buffer_.get () + (4 - putback),
                    this->read_buffer_.get () + 4,
                    this->read_buffer_.get () + 4 + n);

        // return next character
        return char_traits::to_int_type (*this->gptr ());
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    int
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::sync ()
      {
        if (this->pptr () && this->pptr () > this->pbase ())
        {
          if (this->flush_buffer () == -1) return -1;
        }
        return 0;
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    const STREAM_HANDLER&
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::stream () const
      {
        return *this->stream_;
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    void
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::close_stream ()
      {
        if (this->stream_ != 0)
          {
            ACE_Errno_Guard eguard (errno);
            this->stream_->remove_reference ();
            this->stream_ = 0;
          }
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    void
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::set_interceptor (interceptor_type& interceptor)
      {
        this->interceptor_ = &interceptor;
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    void
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::set_mode (openmode mode)
      {
        this->mode_ = mode;
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    typename BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::openmode
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::get_mode () const
      {
        return this->mode_;
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    void
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::reset_buffers()
      {
        this->setg (this->read_buffer_.get () + 4,
                    this->read_buffer_.get () + 4,
                    this->read_buffer_.get () + 4);
        this->setp (this->write_buffer_.get (),
                    this->write_buffer_.get () + (this->bufsize_ - 1));
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    int
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::read_from_stream (char_type* buffer, std::streamsize length)
      {
        return this->stream_ == 0 ? 0 : this->stream_->read_from_stream (buffer, length, sizeof(char_type));
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    int
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::write_to_stream (const char_type* buffer, std::streamsize length)
      {
        return this->stream_ == 0 ? 0 : this->stream_->write_to_stream (buffer, length, sizeof(char_type));
      }

    template <class ACE_CHAR_T, class STREAM_HANDLER, class TR>
    int
    BasicBidirStreamBuffer<ACE_CHAR_T, STREAM_HANDLER, TR>::flush_buffer ()
      {
        int n = int (this->pptr () - this->pbase ());

        if (this->interceptor_)
          this->interceptor_->before_write (this->pbase (), n);

        int n_out = this->write_to_stream (this->pbase (), n);

        if (this->interceptor_)
          this->interceptor_->after_write (n_out);

        if (n_out == n)
        {
          this->pbump (-n);
          return n;
        }
        return -1;
      }

    template <class STREAM_HANDLER>
    BidirStreamBuffer<STREAM_HANDLER>::BidirStreamBuffer (
        STREAM_HANDLER* sh,
        std::streamsize bufsz,
        openmode mode)
      : BasicBidirStreamBuffer<char, STREAM_HANDLER> (sh, bufsz, mode)
      {
      }

    template <class STREAM_HANDLER>
    BidirStreamBuffer<STREAM_HANDLER>::~BidirStreamBuffer ()
      {
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_IOS_BIDIR_STREAM_BUFFER_CPP */
