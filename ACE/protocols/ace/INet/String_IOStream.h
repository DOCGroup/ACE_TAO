/**
 * @file String_IOStream.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_STRING_IOSTREAM_H
#define ACE_IOS_STRING_IOSTREAM_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"
#include "ace/INet/BufferedStreamBuffer.h"
#include "ace/SString.h"
#include <istream>
#include <ostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace IOS
      {
        /**
        * @class ACE_IOS_String_StreamBuffer
        *
        * @brief
        *
        */
        template <class ACE_CHAR_T, class TR = std::char_traits<ACE_CHAR_T> >
        class String_StreamBufferBase
          : public BasicBufferedStreamBuffer<ACE_CHAR_T, TR>
          {
            public:
              using super = BasicBufferedStreamBuffer<ACE_CHAR_T, TR>;
              using char_type = typename super::char_type;
              using openmode = typename super::openmode;
              using string_type = ACE_String_Base<ACE_CHAR_T>;
              using size_type = typename string_type::size_type;
              using pos_type = typename super::pos_type;
              using off_type = typename super::off_type;
              using seekdir = typename super::seekdir;

              String_StreamBufferBase (openmode mode);
              String_StreamBufferBase (string_type& string, openmode mode);
              ~String_StreamBufferBase () override;

              const string_type& str () const;

              void close_string ();

              void clear_string ();

            protected:
              int read_from_stream (char_type* buffer, std::streamsize length) override;

              int write_to_stream (const char_type* buffer, std::streamsize length) override;

              pos_type seekoff (off_type off,
                                seekdir way,
                                openmode which) override;

              pos_type seekpos (pos_type pos,
                                openmode which) override;

            private:
              enum
              {
                BUFFER_SIZE = 1024
              };

              string_type   string_;
              string_type * string_ref_;
              size_type rd_ptr_;
          };

        /**
        * @class ACE_IOS_String_IOS
        *
        * @brief
        *
        */
        template <class ACE_CHAR_T, class TR = std::char_traits<ACE_CHAR_T> >
        class String_IOSBase
          : public virtual std::basic_ios<ACE_CHAR_T, TR>
          {
            public:
              using ios_base = std::basic_ios<ACE_CHAR_T, TR>;
              using buffer_type = String_StreamBufferBase<ACE_CHAR_T, TR>;
              using string_type = typename buffer_type::string_type;
              using openmode = typename buffer_type::openmode;

              String_IOSBase (openmode mode);
              String_IOSBase (string_type& string, openmode mode);
              ~String_IOSBase ();

              buffer_type* rdbuf ();

              void close ();

              const buffer_type& stream () const;

            protected:
              buffer_type streambuf_;
          };

        /**
        * @class ACE_IOS_Sock_OStream
        *
        * @brief
        *
        */
        template <class ACE_CHAR_T, class TR = std::char_traits<ACE_CHAR_T> >
        class String_OStreamBase
          : public String_IOSBase<ACE_CHAR_T, TR>,
            public std::basic_ostream<ACE_CHAR_T, TR>
          {
            public:
              using ios_base = String_IOSBase<ACE_CHAR_T, TR>;
              using string_type = typename ios_base::string_type;

              explicit String_OStreamBase();
              explicit String_OStreamBase(string_type& string);
              ~String_OStreamBase();

              const string_type& str () const;

              void clear ();
          };

        /**
        * @class ACE_IOS_Sock_IStream
        *
        * @brief
        *
        */
        template <class ACE_CHAR_T, class TR = std::char_traits<ACE_CHAR_T> >
        class String_IStreamBase
          : public String_IOSBase<ACE_CHAR_T, TR>,
            public std::basic_istream<ACE_CHAR_T, TR>
          {
            public:
              using ios_base = String_IOSBase<ACE_CHAR_T, TR>;
              using string_type = typename ios_base::string_type;
              using pos_type = typename ios_base::buffer_type::pos_type;

              explicit String_IStreamBase();
              explicit String_IStreamBase(const string_type& string);
              ~String_IStreamBase();

              String_IStreamBase& rewind ();
          };

        using CString_StreamBuffer = String_StreamBufferBase<char>;
        using CString_IOS = String_IOSBase<char>;
        using CString_OStream = String_OStreamBase<char>;
        using CString_IStream = String_IStreamBase<char>;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/INet/String_IOStream.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_IOS_STRING_IOSTREAM_H */
