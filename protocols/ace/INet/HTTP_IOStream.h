// $Id$

/**
 * @file HTTP_IOStream.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_IOSTREAM_H
#define ACE_HTTP_IOSTREAM_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INet/INet_Export.h"
#include "ace/INet/BufferedStreamBuffer.h"
#include "ace/INet/HTTP_StreamPolicyBase.h"
#include <ostream>
#include <istream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {

        /**
        * @class ACE_HTTP_StreamBuffer
        *
        * @brief
        *
        */
        class ACE_INET_Export StreamBuffer
          : public ACE::IOS::BufferedStreamBuffer
          {
            public:
              typedef StreamPolicyBase<StreamBuffer>  policy_type;

              StreamBuffer (std::iostream& stream, policy_type* policy = 0);
              virtual ~StreamBuffer ();

              virtual int sync ();

            protected:
              friend class StreamPolicyBase<StreamBuffer>;

              enum
                {
                  BUFFER_SIZE = 4096
                };

              virtual int read_from_stream (char* buffer, std::streamsize length);

              virtual int write_to_stream (const char* buffer, std::streamsize length);

              virtual int read_from_stream_i (char* buffer, std::streamsize length);

              virtual int write_to_stream_i (const char* buffer, std::streamsize length);

            private:
              std::iostream & stream_;
              policy_type * policy_;
          };


        /**
        * @class ACE_HTTP_IOS
        *
        * @brief
        *
        */
        class ACE_INET_Export IOS
          : public virtual std::ios
          {
            public:
              IOS (std::iostream& stream, StreamBuffer::policy_type* policy = 0);
              ~IOS ();

              StreamBuffer* rdbuf ();

            protected:
              StreamBuffer streambuf_;
          };

        /**
        * @class ACE_HTTP_OStream
        *
        * @brief
        *
        */
        class ACE_INET_Export OStream
          : public IOS, public std::ostream
          {
            public:
              explicit OStream(std::iostream& stream, StreamBuffer::policy_type* policy = 0);
              ~OStream();

              void set_interceptor (StreamBuffer::interceptor_type& interceptor);
          };

        /**
        * @class ACE_HTTP_IStream
        *
        * @brief
        *
        */
        class ACE_INET_Export IStream
          : public IOS, public std::istream
          {
            public:
              explicit IStream(std::iostream& stream, StreamBuffer::policy_type* policy = 0);
              ~IStream();

              void set_interceptor (StreamBuffer::interceptor_type& interceptor);
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_IOStream.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_IOSTREAM_H */
