// $Id$

/**
 * @file BidirStreamBuffer.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_BIDIR_STREAM_BUFFER_H
#define ACE_IOS_BIDIR_STREAM_BUFFER_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "ace/INet/StreamInterceptor.h"
#include <streambuf>
#include <iosfwd>
#include <ios>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace IOS
      {
        /**
        * @class ACE_IOS_BasicBidirStreamBuffer
        *
        * @brief Encapsulates bidirectional streambuffer attached
        * to a StreamHandler implementation.
        *
        * Implements a C++ standard bidirectional streambuffer using
        * a streamed connection handler as data source/destination.
        * Used as a base for implementing C++ standard streams capable
        * of performing C++ standard formatted input and output on
        * any streamed connection that can be represented by a stream
        * handler.
        */
        template <class ACE_CHAR_T, class STREAM_HANDLER, class TR = std::char_traits<ACE_CHAR_T> >
        class BasicBidirStreamBuffer
          : public std::basic_streambuf<ACE_CHAR_T, TR>
          {
            public:
              // useful traits
              typedef std::basic_streambuf<ACE_CHAR_T, TR> base_type;
              typedef std::basic_ios<ACE_CHAR_T, TR> ios_type;
              typedef ACE_CHAR_T char_type;
              typedef TR char_traits;
              typedef typename base_type::int_type int_type;
              typedef typename base_type::pos_type pos_type;
              typedef typename base_type::off_type off_type;
              typedef typename ios_type::openmode openmode;
              typedef StreamInterceptorBase<char_type, char_traits> interceptor_type;

              /// Constructor
              BasicBidirStreamBuffer (STREAM_HANDLER* sh, std::streamsize bufsz, openmode mode);

              /// Destructor
              virtual ~BasicBidirStreamBuffer ();

              virtual int_type overflow (int_type c);
              virtual int_type underflow ();
              virtual int sync ();

              const STREAM_HANDLER& stream () const;

              void close_stream ();

              void set_interceptor (interceptor_type& interceptor);

            protected:
              void set_mode (openmode mode);

              openmode get_mode () const;

              void reset_buffers();

            private:
              virtual int read_from_stream (char_type* buffer, std::streamsize length);

              virtual int write_to_stream (const char_type* buffer, std::streamsize length);

              int flush_buffer ();

              std::streamsize bufsize_;
              ACE_Auto_Array_Ptr<char_type> read_buffer_;
              ACE_Auto_Array_Ptr<char_type> write_buffer_;
              openmode        mode_;
              STREAM_HANDLER *stream_;
              interceptor_type* interceptor_;

              BasicBidirStreamBuffer(const BasicBidirStreamBuffer&);
              BasicBidirStreamBuffer& operator = (const BasicBidirStreamBuffer&);
          };

        /**
        * @class ACE_IOS_BidirStreamBuffer
        *
        * @brief Encapsulates bidirectional character streambuffer attached
        * to a StreamHandler implementation.
        *
        */
        template <class STREAM_HANDLER>
        class BidirStreamBuffer
          : public BasicBidirStreamBuffer<char, STREAM_HANDLER>
          {
            public:
              typedef BasicBidirStreamBuffer< char, STREAM_HANDLER> super;
              typedef typename super::openmode openmode;

              BidirStreamBuffer (STREAM_HANDLER* sh, std::streamsize bufsz, openmode mode);
              virtual ~BidirStreamBuffer ();

            private:
              BidirStreamBuffer(const BidirStreamBuffer&);
              BidirStreamBuffer& operator = (const BidirStreamBuffer&);
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/BidirStreamBuffer.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("BidirStreamBuffer.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_IOS_BIDIR_STREAM_BUFFER_H */
