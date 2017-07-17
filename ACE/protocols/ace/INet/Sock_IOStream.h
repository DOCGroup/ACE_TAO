/**
 * @file Sock_IOStream.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_SOCK_IOSTREAM_H
#define ACE_IOS_SOCK_IOSTREAM_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
        * @class ACE_IOS_Sock_StreamBuffer
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class Sock_StreamBufferBase
          : public BidirStreamBuffer<StreamHandler<ACE_SOCK_STREAM, ACE_SYNCH_USE> >
          {
            public:
              typedef StreamHandler<ACE_SOCK_STREAM, ACE_SYNCH_USE> stream_type;

              Sock_StreamBufferBase (stream_type* stream);
              virtual ~Sock_StreamBufferBase ();

            private:
              enum
              {
                BUFFER_SIZE = 1024
              };
          };

        /**
        * @class ACE_IOS_Sock_IOS
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class Sock_IOSBase
          : public virtual std::ios
          {
            public:
              typedef Sock_StreamBufferBase<ACE_SYNCH_USE> buffer_type;
              typedef typename buffer_type::stream_type stream_type;

              Sock_IOSBase (stream_type* stream);
              ~Sock_IOSBase ();

              buffer_type* rdbuf ();

              void close ();

              const stream_type& stream () const;

            protected:
              buffer_type streambuf_;
          };

        /**
        * @class ACE_IOS_Sock_OStream
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class Sock_OStreamBase
          : public Sock_IOSBase<ACE_SYNCH_USE>, public std::ostream
          {
            public:
              typedef Sock_IOSBase<ACE_SYNCH_USE> ios_base;
              typedef typename ios_base::stream_type stream_type;
              typedef Sock_StreamBufferBase<ACE_SYNCH_USE> buffer_type;

              explicit Sock_OStreamBase(stream_type* stream);

              ~Sock_OStreamBase();

              buffer_type* rdbuf ()
              { return ios_base::rdbuf (); }

              void set_interceptor (typename buffer_type::interceptor_type& interceptor);
          };

        /**
        * @class ACE_IOS_Sock_IStream
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class Sock_IStreamBase
          : public Sock_IOSBase<ACE_SYNCH_USE>, public std::istream
          {
            public:
              typedef Sock_IOSBase<ACE_SYNCH_USE> ios_base;
              typedef typename ios_base::stream_type stream_type;
              typedef Sock_StreamBufferBase<ACE_SYNCH_USE> buffer_type;

              explicit Sock_IStreamBase(stream_type* stream);

              buffer_type* rdbuf ()
              { return ios_base::rdbuf (); }

              ~Sock_IStreamBase();

              void set_interceptor (typename buffer_type::interceptor_type& interceptor);
          };

        /**
        * @class ACE_IOS_Sock_IOStream
        *
        * @brief
        *
        */
        template <ACE_SYNCH_DECL>
        class Sock_IOStreamBase
          : public Sock_IOSBase<ACE_SYNCH_USE>, public std::iostream
          {
            public:
              typedef Sock_IOSBase<ACE_SYNCH_USE> ios_base;
              typedef typename ios_base::stream_type stream_type;
              typedef Sock_StreamBufferBase<ACE_SYNCH_USE> buffer_type;

              explicit Sock_IOStreamBase(stream_type* stream);

              buffer_type* rdbuf ()
              { return ios_base::rdbuf (); }

              ~Sock_IOStreamBase();

              void set_interceptor (typename buffer_type::interceptor_type& interceptor);
          };

        typedef Sock_StreamBufferBase<ACE_NULL_SYNCH> Sock_StreamBuffer;
        typedef Sock_IOSBase<ACE_NULL_SYNCH> Sock_IOS;
        typedef Sock_IStreamBase<ACE_NULL_SYNCH> Sock_IStream;
        typedef Sock_OStreamBase<ACE_NULL_SYNCH> Sock_OStream;
        typedef Sock_IOStreamBase<ACE_NULL_SYNCH> Sock_IOStream;

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/Sock_IOStream.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Sock_IOStream.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_IOS_SOCK_IOSTREAM_H */
