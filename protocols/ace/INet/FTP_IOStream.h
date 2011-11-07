// $Id$

/**
 * @file FTP_IOStream.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_FTP_IOSTREAM_H
#define ACE_FTP_IOSTREAM_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INet/INet_Export.h"
#include "ace/INet/BufferedStreamBuffer.h"
#include <ostream>
#include <istream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace FTP
      {

        /**
        * @class ACE_FTP_StreamBuffer
        *
        * @brief
        *
        */
        class ACE_INET_Export StreamBuffer
          : public ACE::IOS::BufferedStreamBuffer
          {
            public:
              StreamBuffer (std::iostream * stream);
              virtual ~StreamBuffer ();

              virtual int sync ();

              std::iostream* set_stream (std::iostream * stream);

            protected:
              enum
                {
                  BUFFER_SIZE = 4096
                };

              virtual int read_from_stream (char* buffer, std::streamsize length);

              virtual int write_to_stream (const char* buffer, std::streamsize length);

            private:
              std::iostream * stream_;
          };


        /**
        * @class ACE_FTP_IOS
        *
        * @brief
        *
        */
        class ACE_INET_Export IOS
          : public virtual std::ios
          {
            public:
              IOS (std::iostream* stream);
              ~IOS ();

              StreamBuffer* rdbuf ();

              std::iostream* set_stream (std::iostream* stream);

            protected:
              StreamBuffer streambuf_;
          };

        /**
        * @class ACE_FTP_OStream
        *
        * @brief
        *
        */
        class ACE_INET_Export OStream
          : public IOS, public std::ostream
          {
            public:
              explicit OStream(std::iostream* stream);
              ~OStream();

              void set_interceptor (StreamBuffer::interceptor_type& interceptor);
          };

        /**
        * @class ACE_FTP_IStream
        *
        * @brief
        *
        */
        class ACE_INET_Export IStream
          : public IOS, public std::istream
          {
            public:
              explicit IStream(std::iostream* stream);
              ~IStream();

              void set_interceptor (StreamBuffer::interceptor_type& interceptor);
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/FTP_IOStream.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_FTP_IOSTREAM_H */
