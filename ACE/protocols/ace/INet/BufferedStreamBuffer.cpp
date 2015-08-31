#ifndef ACE_BUFFERED_STREAM_BUFFER_CPP
#define ACE_BUFFERED_STREAM_BUFFER_CPP

#include "ace/INet/BufferedStreamBuffer.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {

    template <class ACE_CHAR_T, class TR>
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::BasicBufferedStreamBuffer (
        std::streamsize bufsz,
        typename std::basic_ios<ACE_CHAR_T, TR>::openmode mode)
      : bufsize_ (bufsz),
        mode_ (mode),
        interceptor_ (0)
      {
        char_type* p = 0;
        ACE_NEW_NORETURN (p, char_type [bufsz]);
        this->buffer_.reset (p);

        this->setg (this->buffer_.get () + 4,
                    this->buffer_.get () + 4,
                    this->buffer_.get () + 4);
        this->setp (this->buffer_.get (),
                    this->buffer_.get () + (this->bufsize_ - 1));
      }

    template <class ACE_CHAR_T, class TR>
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::~BasicBufferedStreamBuffer ()
      {
      }

    template <class ACE_CHAR_T, class TR>
    typename BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::int_type
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::overflow (int_type c)
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

    template <class ACE_CHAR_T, class TR>
    typename BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::int_type
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::underflow ()
      {
        if (!(this->mode_ & ios_type::in)) return char_traits::eof ();

        if (this->gptr () && (this->gptr () < this->egptr ()))
          return char_traits::to_int_type (*this->gptr ());

        int putback = int (this->gptr () - this->eback ());
        if (putback > 4) putback = 4;

        ACE_OS::memmove (this->buffer_.get () + (4 - putback),
                         this->gptr () - putback,
                         putback * sizeof (char_type));

        if (this->interceptor_)
          this->interceptor_->before_read (this->bufsize_ - 4);

        int n = this->read_from_stream (this->buffer_.get () + 4,
                                        this->bufsize_ - 4);

        if (this->interceptor_)
          this->interceptor_->after_read (this->buffer_.get () + 4, n);

        if (n <= 0)
          {
            if (this->interceptor_)
              this->interceptor_->on_eof ();

            return char_traits::eof ();
          }

        this->setg (this->buffer_.get () + (4 - putback),
                    this->buffer_.get () + 4,
                    this->buffer_.get () + 4 + n);

        // return next character
        return char_traits::to_int_type (*this->gptr ());
      }

    template <class ACE_CHAR_T, class TR>
    int
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::sync ()
      {
        if (this->pptr () && this->pptr () > this->pbase ())
        {
          if (this->flush_buffer () == -1) return -1;
        }
        return 0;
      }

    template <class ACE_CHAR_T, class TR>
    void
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::set_interceptor (interceptor_type& interceptor)
      {
        this->interceptor_ = &interceptor;
      }

    template <class ACE_CHAR_T, class TR>
    void
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::set_mode (
        typename std::basic_ios<ACE_CHAR_T, TR>::openmode mode)
      {
        this->mode_ = mode;
      }

    template <class ACE_CHAR_T, class TR>
    typename std::basic_ios<ACE_CHAR_T, TR>::openmode
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::get_mode () const
      {
        return this->mode_;
      }

    template <class ACE_CHAR_T, class TR>
    int
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::read_from_stream (char_type* /*buffer*/, std::streamsize /*length*/)
      {
        // to be implemented in derived class
        return 0;
      }

    template <class ACE_CHAR_T, class TR>
    int
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::write_to_stream (const char_type* /*buffer*/, std::streamsize /*length*/)
      {
        // to be implemented in derived class
        return 0;
      }

    template <class ACE_CHAR_T, class TR>
    void
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::reset_buffers()
      {
        this->setg (this->buffer_.get () + 4,
                    this->buffer_.get () + 4,
                    this->buffer_.get () + 4);
        this->setp (this->buffer_.get (),
                    this->buffer_.get () + (this->bufsize_ - 1));
      }

    template <class ACE_CHAR_T, class TR>
    int
    BasicBufferedStreamBuffer<ACE_CHAR_T, TR>::flush_buffer ()
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
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_BUFFERED_STREAM_BUFFER_CPP */
