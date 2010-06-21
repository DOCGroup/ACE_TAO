// $Id$

#ifndef ACE_HTTP_STREAM_POLICY_CPP
#define ACE_HTTP_STREAM_POLICY_CPP

#include "ace/INet/HTTP_StreamPolicy.h"
#include <cctype>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    template <class STREAM_BUFFER>
    FixedLengthStreamPolicyBase<STREAM_BUFFER>::FixedLengthStreamPolicyBase (std::streamsize length)
      : StreamPolicyBase<STREAM_BUFFER> (),
        length_ (length),
        count_ (0)
      {
      }

    template <class STREAM_BUFFER>
    FixedLengthStreamPolicyBase<STREAM_BUFFER>::~FixedLengthStreamPolicyBase ()
      {
      }

    template <class STREAM_BUFFER>
    int FixedLengthStreamPolicyBase<STREAM_BUFFER>::read_from_stream (
        char_type * buf,
        std::streamsize length)
      {
        int n = 0;
        if (this->count_ < this->length_)
        {
          if (this->count_ + length > this->length_)
            length = this->length_ - this->count_;
          n = this->read_from_stream_i (buf, length);
          if (n > 0) this->count_ += n;
        }
        return n;
      }

    template <class STREAM_BUFFER>
    int FixedLengthStreamPolicyBase<STREAM_BUFFER>::write_to_stream (
        const char_type * buf,
        std::streamsize length)
      {
        int n = 0;
        if (this->count_ < this->length_)
        {
          if ((this->count_ + length) > this->length_)
            length = this->length_ - this->count_;
          n = this->write_to_stream_i (buf, length);
          if (n > 0) this->count_ += n;
        }
        return n;
      }

    template <class STREAM_BUFFER>
    ChunkedTransferStreamPolicyBase<STREAM_BUFFER>::ChunkedTransferStreamPolicyBase ()
      : StreamPolicyBase<STREAM_BUFFER> (),
        chunk_cnt_ (0)
      {
      }

    template <class STREAM_BUFFER>
    ChunkedTransferStreamPolicyBase<STREAM_BUFFER>::~ChunkedTransferStreamPolicyBase ()
      {
      }

    template <class STREAM_BUFFER>
    int ChunkedTransferStreamPolicyBase<STREAM_BUFFER>::getc ()
      {
        static const int eof_ =
          std::char_traits<char_type>::eof ();

        char_type chbuf[1];
        if (this->read_from_stream_i (chbuf, 1) <= 0)
          return eof_;
        else
          return chbuf[0];
      }

    template <class STREAM_BUFFER>
    int ChunkedTransferStreamPolicyBase<STREAM_BUFFER>::read_from_stream (
        char_type * buf,
        std::streamsize length)
      {
        static const int eof_ =
          std::char_traits<char_type>::eof ();

        char_type lf = this->chunk_.widen ('\n');
        if (this->chunk_cnt_ == 0)
        {
          int ch = this->getc ();
          while (std::isspace (ch))
            ch = this->getc ();
          typename buffer_type::string_type chunk_len_str;
          while (std::isxdigit (ch))
            {
              chunk_len_str += (char_type) ch;
              ch = this->getc ();
            }
          while (ch != eof_ && ch != lf)
            {
              ch = this->getc ();
            }
          ACE::IOS::String_IStreamBase<char_type> chunk_len_is (chunk_len_str);
          unsigned chunk_len;
          if (!(chunk_len_is >> chunk_len))
            return eof_;
          this->chunk_cnt_ = (std::streamsize) chunk_len;
        }
        if (this->chunk_cnt_ > 0)
        {
          if (length > this->chunk_cnt_)
            length = this->chunk_cnt_;
          int n = this->read_from_stream_i (buf, length);
          if (n > 0)
            this->chunk_cnt_ -= n;
          return n;
        }
        else
        {
          int ch = this->getc ();
          while (ch != eof_ && ch != lf)
            ch = this->getc ();
          return 0;
        }
      }

    template <class STREAM_BUFFER>
    int ChunkedTransferStreamPolicyBase<STREAM_BUFFER>::write_to_stream (
        const char_type * buf,
        std::streamsize length)
      {
        this->chunk_.clear ();
        this->chunk_ << std::hex << length << std::dec;
        this->chunk_ << this->chunk_.widen ('\r') << this->chunk_.widen ('\n');
        this->chunk_.write (buf, length);
        this->chunk_ << this->chunk_.widen ('\r') << this->chunk_.widen ('\n');
        const typename buffer_type::string_type& str = this->chunk_.str ();
        return this->write_to_stream_i (str.c_str (), str.length ());
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HTTP_STREAM_POLICY_CPP */
