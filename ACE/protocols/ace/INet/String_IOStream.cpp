// $Id$

#ifndef ACE_IOS_STRING_IOSTREAM_CPP
#define ACE_IOS_STRING_IOSTREAM_CPP

#include "String_IOStream.h"
#include "IOS_util.h"

ACE_RCSID(NET_CLIENT,ACE_IOS_String_IOStream,"$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {

    template <class ACE_CHAR_T, class TR>
    String_StreamBufferBase<ACE_CHAR_T, TR>::String_StreamBufferBase (openmode mode)
      : BasicBufferedStreamBuffer<ACE_CHAR_T, TR> (BUFFER_SIZE, mode),
        string_ref_ (&string_),
        rd_ptr_ (0)
      {
      }

    template <class ACE_CHAR_T, class TR>
    String_StreamBufferBase<ACE_CHAR_T, TR>::String_StreamBufferBase (string_type& string, openmode mode)
      : BasicBufferedStreamBuffer<ACE_CHAR_T, TR> (BUFFER_SIZE, mode),
        string_ref_ (&string),
        rd_ptr_ (0)
      {
      }

    template <class ACE_CHAR_T, class TR>
    String_StreamBufferBase<ACE_CHAR_T, TR>::~String_StreamBufferBase ()
      {
      }

    template <class ACE_CHAR_T, class TR>
    const typename String_StreamBufferBase<ACE_CHAR_T, TR>::string_type&
    String_StreamBufferBase<ACE_CHAR_T, TR>::str () const
      {
        const_cast<String_StreamBufferBase<ACE_CHAR_T, TR>*> (this)->sync ();
        return *this->string_ref_;
      }

    template <class ACE_CHAR_T, class TR>
    void String_StreamBufferBase<ACE_CHAR_T, TR>::close_string ()
      {
        this->sync ();
        this->string_ref_ = 0;
      }

    template <class ACE_CHAR_T, class TR>
    void String_StreamBufferBase<ACE_CHAR_T, TR>::clear_string ()
      {
        this->sync ();
        this->string_ref_->clear ();
      }

    template <class ACE_CHAR_T, class TR>
    int String_StreamBufferBase<ACE_CHAR_T, TR>::read_from_stream (char_type* buffer, std::streamsize length)
      {
        int n = 0;
        if (this->string_ref_)
          {
            if ((this->rd_ptr_ + length) > this->string_ref_->length ())
              {
                length = this->string_ref_->length () - this->rd_ptr_;
              }
            ACE_OS::memmove (buffer, &(*this->string_ref_)[this->rd_ptr_], length * sizeof (char_type));
            this->rd_ptr_ += length;
            n = length;
          }
        return n;
      }

    template <class ACE_CHAR_T, class TR>
    int String_StreamBufferBase<ACE_CHAR_T, TR>::write_to_stream (const char_type* buffer, std::streamsize length)
      {
        int n = 0;
        if (this->string_ref_)
          {
            this->string_ref_->append (buffer, length);
            n = length;
          }
        return n;
      }

    template <class ACE_CHAR_T, class TR>
    String_IOSBase<ACE_CHAR_T, TR>::String_IOSBase (openmode mode)
      : streambuf_ (mode)
      {
        ace_ios_init (&this->streambuf_);
      }

    template <class ACE_CHAR_T, class TR>
    String_IOSBase<ACE_CHAR_T, TR>::String_IOSBase (string_type& string, openmode mode)
      : streambuf_ (string, mode)
      {
        ace_ios_init (&this->streambuf_);
      }

    template <class ACE_CHAR_T, class TR>
    String_IOSBase<ACE_CHAR_T, TR>::~String_IOSBase ()
      {
      }

    template <class ACE_CHAR_T, class TR>
    typename String_IOSBase<ACE_CHAR_T, TR>::buffer_type*
    String_IOSBase<ACE_CHAR_T, TR>::rdbuf ()
      {
        return &this->streambuf_;
      }

    template <class ACE_CHAR_T, class TR>
    void String_IOSBase<ACE_CHAR_T, TR>::close ()
      {
        this->streambuf_.close_string ();
      }

    template <class ACE_CHAR_T, class TR>
    const typename String_IOSBase<ACE_CHAR_T, TR>::buffer_type&
    String_IOSBase<ACE_CHAR_T, TR>::stream () const
      {
        return this->streambuf_;
      }

    template <class ACE_CHAR_T, class TR>
    String_OStreamBase<ACE_CHAR_T, TR>::String_OStreamBase()
      : String_IOSBase<ACE_CHAR_T, TR> (std::ios::out),
        std::basic_ostream<ACE_CHAR_T, TR> (String_IOSBase<ACE_CHAR_T, TR>::rdbuf ())
      {
      }

    template <class ACE_CHAR_T, class TR>
    String_OStreamBase<ACE_CHAR_T, TR>::String_OStreamBase(string_type& string)
      : String_IOSBase<ACE_CHAR_T, TR> (string, std::ios::out),
        std::basic_ostream<ACE_CHAR_T, TR> (String_IOSBase<ACE_CHAR_T, TR>::rdbuf ())
      {
      }

    template <class ACE_CHAR_T, class TR>
    String_OStreamBase<ACE_CHAR_T, TR>::~String_OStreamBase()
      {
      }

    template <class ACE_CHAR_T, class TR>
    const typename String_OStreamBase<ACE_CHAR_T, TR>::string_type&
    String_OStreamBase<ACE_CHAR_T, TR>::str () const
      {
        return this->stream ().str ();
      }

    template <class ACE_CHAR_T, class TR>
    void String_OStreamBase<ACE_CHAR_T, TR>::clear ()
      {
        return this->rdbuf ()->clear_string ();
      }

    template <class ACE_CHAR_T, class TR>
    String_IStreamBase<ACE_CHAR_T, TR>::String_IStreamBase()
      : String_IOSBase<ACE_CHAR_T, TR> (std::ios::in),
        std::basic_istream<ACE_CHAR_T, TR> (String_IOSBase<ACE_CHAR_T, TR>::rdbuf ())
      {
      }

    template <class ACE_CHAR_T, class TR>
    String_IStreamBase<ACE_CHAR_T, TR>::String_IStreamBase(string_type& string)
      : String_IOSBase<ACE_CHAR_T, TR> (string, std::ios::in),
        std::basic_istream<ACE_CHAR_T, TR> (String_IOSBase<ACE_CHAR_T, TR>::rdbuf ())
      {
      }

    template <class ACE_CHAR_T, class TR>
    String_IStreamBase<ACE_CHAR_T, TR>::~String_IStreamBase()
      {
      }
  };
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_IOS_STRING_IOSTREAM_CPP */
