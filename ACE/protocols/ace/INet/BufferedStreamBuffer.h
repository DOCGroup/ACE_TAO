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

#include "ace/INet/StreamInterceptor.h"
#include <streambuf>
#include <iosfwd>
#include <ios>
#include <memory>

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
              using base_type = std::basic_streambuf<ACE_CHAR_T, TR>;
              using ios_type = std::basic_ios<ACE_CHAR_T, TR>;
              using char_type = ACE_CHAR_T;
              using char_traits = TR;
              using int_type = typename base_type::int_type;
              using pos_type = typename base_type::pos_type;
              using off_type = typename base_type::off_type;
              using seekdir = typename ios_type::seekdir;
              using openmode = typename ios_type::openmode;
              using interceptor_type = StreamInterceptorBase<char_type, char_traits>;

              BasicBufferedStreamBuffer (std::streamsize bufsz,
                                         typename std::basic_ios<ACE_CHAR_T, TR>::openmode mode);

              ~BasicBufferedStreamBuffer () override;

              int_type overflow (int_type c) override;
              int_type underflow () override;
              int sync () override;

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
              std::unique_ptr<char_type[]> buffer_;
              typename std::basic_ios<ACE_CHAR_T, TR>::openmode mode_;
              interceptor_type* interceptor_;

              BasicBufferedStreamBuffer(const BasicBufferedStreamBuffer&);
              BasicBufferedStreamBuffer& operator = (const BasicBufferedStreamBuffer&);
          };

        /// char specialization
        using BufferedStreamBuffer = BasicBufferedStreamBuffer<char, std::char_traits<char> >;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/INet/BufferedStreamBuffer.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_IOS_BUFFERED_STREAM_BUFFER_H */
