/**
 * @file SSLSock_IOStream.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_SSLSOCK_IOSTREAM_H
#define ACE_IOS_SSLSOCK_IOSTREAM_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SSL/SSL_SOCK_Stream.h"
#include "ace/INet/BidirStreamBuffer.h"
#include "ace/INet/StreamHandler.h"
#include "ace/INet/StreamInterceptor.h"
#include <istream>
#include <ostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace IOS
      {
        /**
        * @class ACE_IOS_SSLSock_StreamBuffer
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class SSLSock_StreamBufferBase
          : public BidirStreamBuffer<StreamHandler<ACE_SSL_SOCK_Stream, ACE_SYNCH_USE> >
          {
            public:
              using stream_type = StreamHandler<ACE_SSL_SOCK_Stream, ACE_SYNCH_USE>;

              SSLSock_StreamBufferBase (stream_type* stream);
              virtual ~SSLSock_StreamBufferBase ();

            private:
              enum
              {
                BUFFER_SIZE = 1024
              };
          };

        /**
        * @class ACE_IOS_SSLSock_IOS
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class SSLSock_IOSBase
          : public virtual std::ios
          {
            public:
              using buffer_type = SSLSock_StreamBufferBase<ACE_SYNCH_USE>;
              using stream_type = typename buffer_type::stream_type;

              SSLSock_IOSBase (stream_type* stream);
              ~SSLSock_IOSBase ();

              buffer_type* rdbuf ();

              void close ();

              const stream_type& stream () const;

            protected:
              buffer_type streambuf_;
          };

        /**
        * @class ACE_IOS_SSLSock_OStream
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class SSLSock_OStreamBase
          : public SSLSock_IOSBase<ACE_SYNCH_USE>, public std::ostream
          {
            public:
              using ios_base = SSLSock_IOSBase<ACE_SYNCH_USE>;
              using stream_type = typename ios_base::stream_type;
              using buffer_type = SSLSock_StreamBufferBase<ACE_SYNCH_USE>;

              explicit SSLSock_OStreamBase(stream_type* stream);

              buffer_type* rdbuf ()
              { return ios_base::rdbuf (); }

              ~SSLSock_OStreamBase();

              void set_interceptor (typename buffer_type::interceptor_type& interceptor);
          };

        /**
        * @class ACE_IOS_SSLSock_IStream
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class SSLSock_IStreamBase
          : public SSLSock_IOSBase<ACE_SYNCH_USE>, public std::istream
          {
            public:
              using ios_base = SSLSock_IOSBase<ACE_SYNCH_USE>;
              using stream_type = typename ios_base::stream_type;
              using buffer_type = SSLSock_StreamBufferBase<ACE_SYNCH_USE>;

              explicit SSLSock_IStreamBase(stream_type* stream);

              buffer_type* rdbuf ()
              { return ios_base::rdbuf (); }

              ~SSLSock_IStreamBase();

              void set_interceptor (typename buffer_type::interceptor_type& interceptor);
          };

        /**
        * @class ACE_IOS_SSLSock_IOStream
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class SSLSock_IOStreamBase
          : public SSLSock_IOSBase<ACE_SYNCH_USE>, public std::iostream
          {
            public:
              using ios_base = SSLSock_IOSBase<ACE_SYNCH_USE>;
              using stream_type = typename ios_base::stream_type;
              using buffer_type = SSLSock_StreamBufferBase<ACE_SYNCH_USE>;

              explicit SSLSock_IOStreamBase(stream_type* stream);

              buffer_type* rdbuf ()
              { return ios_base::rdbuf (); }

              ~SSLSock_IOStreamBase();

              void set_interceptor (typename buffer_type::interceptor_type& interceptor);
          };

        using SSLSock_StreamBuffer = SSLSock_StreamBufferBase<ACE_NULL_SYNCH>;
        using SSLSock_IOS = SSLSock_IOSBase<ACE_NULL_SYNCH>;
        using SSLSock_IStream = SSLSock_IStreamBase<ACE_NULL_SYNCH>;
        using SSLSock_OStream = SSLSock_OStreamBase<ACE_NULL_SYNCH>;
        using SSLSock_IOStream = SSLSock_IOStreamBase<ACE_NULL_SYNCH>;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/INet/SSLSock_IOStream.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_IOS_SSLSOCK_IOSTREAM_H */
