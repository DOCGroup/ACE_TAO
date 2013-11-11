// $Id$

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
              typedef BasicBufferedStreamBuffer<ACE_CHAR_T, TR> super;
              typedef typename super::char_type char_type;
              typedef typename super::openmode openmode;
              typedef ACE_String_Base<ACE_CHAR_T> string_type;
              typedef typename string_type::size_type size_type;
              typedef typename super::pos_type pos_type;
              typedef typename super::off_type off_type;
              typedef typename super::seekdir seekdir;

              String_StreamBufferBase (openmode mode);
              String_StreamBufferBase (string_type& string, openmode mode);
              virtual ~String_StreamBufferBase ();

              const string_type& str () const;

              void close_string ();

              void clear_string ();

            protected:
              virtual int read_from_stream (char_type* buffer, std::streamsize length);

              virtual int write_to_stream (const char_type* buffer, std::streamsize length);

              virtual pos_type seekoff (off_type off,
                                        seekdir way,
                                        openmode which);

              virtual pos_type seekpos (pos_type pos,
                                        openmode which);

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
              typedef std::basic_ios<ACE_CHAR_T, TR> ios_base;
              typedef String_StreamBufferBase<ACE_CHAR_T, TR> buffer_type;
              typedef typename buffer_type::string_type string_type;
              typedef typename buffer_type::openmode openmode;

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
              typedef String_IOSBase<ACE_CHAR_T, TR> ios_base;
              typedef typename ios_base::string_type string_type;

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
              typedef String_IOSBase<ACE_CHAR_T, TR> ios_base;
              typedef typename ios_base::string_type string_type;
              typedef typename ios_base::buffer_type::pos_type pos_type;

              explicit String_IStreamBase();
              explicit String_IStreamBase(const string_type& string);
              ~String_IStreamBase();

              String_IStreamBase& rewind ();
          };

        typedef String_StreamBufferBase<char> CString_StreamBuffer;
        typedef String_IOSBase<char> CString_IOS;
        typedef String_OStreamBase<char> CString_OStream;
        typedef String_IStreamBase<char> CString_IStream;

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/String_IOStream.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("String_IOStream.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_IOS_STRING_IOSTREAM_H */
