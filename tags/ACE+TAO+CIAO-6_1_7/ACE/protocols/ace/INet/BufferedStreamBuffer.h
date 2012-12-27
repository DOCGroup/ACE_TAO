// $Id$

/**
 * @file BufferedStreamBuffer.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_BUFFERED_STREAM_BUFFER_H
#define ACE_IOS_BUFFERED_STREAM_BUFFER_H

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
        * @class ACE_IOS_BasicBufferedStreamBuffer
        *
        * @brief Encapsulates unidirectional streambuffer attached
        * to an input OR output stream.
        *
        * Implements a base class for a C++ standard unidirectional
        * streambuffer using any data source/destination that can be
        * driven by overloading the <write_to_stream> or <read_from_stream>
        * methods.
        */
        template <class ACE_CHAR_T, class TR = std::char_traits<ACE_CHAR_T> >
        class BasicBufferedStreamBuffer
          : public std::basic_streambuf<ACE_CHAR_T, TR>
          {
            public:
              typedef std::basic_streambuf<ACE_CHAR_T, TR> base_type;
              typedef std::basic_ios<ACE_CHAR_T, TR> ios_type;
              typedef ACE_CHAR_T char_type;
              typedef TR char_traits;
              typedef typename base_type::int_type int_type;
              typedef typename base_type::pos_type pos_type;
              typedef typename base_type::off_type off_type;
              typedef typename ios_type::seekdir seekdir;
              typedef typename ios_type::openmode openmode;
              typedef StreamInterceptorBase<char_type, char_traits> interceptor_type;

              BasicBufferedStreamBuffer (std::streamsize bufsz,
                                         typename std::basic_ios<ACE_CHAR_T, TR>::openmode mode);

              virtual ~BasicBufferedStreamBuffer ();

              virtual int_type overflow (int_type c);
              virtual int_type underflow ();
              virtual int sync ();

              void set_interceptor (interceptor_type& interceptor);

            protected:
              void set_mode (typename std::basic_ios<ACE_CHAR_T, TR>::openmode mode);

              typename std::basic_ios<ACE_CHAR_T, TR>::openmode get_mode () const;

              virtual int read_from_stream (char_type* buffer, std::streamsize length);

              virtual int write_to_stream (const char_type* buffer, std::streamsize length);

              void reset_buffers ();

            private:
              int flush_buffer ();

              std::streamsize bufsize_;
              ACE_Auto_Array_Ptr<char_type> buffer_;
              typename std::basic_ios<ACE_CHAR_T, TR>::openmode mode_;
              interceptor_type* interceptor_;

              BasicBufferedStreamBuffer(const BasicBufferedStreamBuffer&);
              BasicBufferedStreamBuffer& operator = (const BasicBufferedStreamBuffer&);
          };

        /// char specialization
        typedef BasicBufferedStreamBuffer<char, std::char_traits<char> > BufferedStreamBuffer;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/BufferedStreamBuffer.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("BufferedStreamBuffer.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_IOS_BUFFERED_STREAM_BUFFER_H */
