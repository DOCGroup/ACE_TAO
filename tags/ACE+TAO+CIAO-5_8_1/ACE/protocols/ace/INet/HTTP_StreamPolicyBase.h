// $Id$

/**
 * @file HTTP_StreamPolicyBase.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_STREAM_POLICY_BASE_H
#define ACE_HTTP_STREAM_POLICY_BASE_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"
#include "ace/INet/BufferedStreamBuffer.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_IOS_StreamPolicyBase
        *
        * @brief Abstract base for HTTP stream policies.
        *
        */
        template <class STREAM_BUFFER>
        class StreamPolicyBase
          {
            public:
              StreamPolicyBase ();
              virtual ~StreamPolicyBase ();

              typedef typename STREAM_BUFFER::char_type char_type;

              virtual int read_from_stream (char_type* buffer, std::streamsize length) = 0;

              virtual int write_to_stream (const char_type* buffer, std::streamsize length) = 0;

              void set_stream_buffer (STREAM_BUFFER* streambuf);

            protected:
              int read_from_stream_i (char_type* buffer, std::streamsize length);

              int write_to_stream_i (const char_type* buffer, std::streamsize length);

            private:
              STREAM_BUFFER* streambuf_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/HTTP_StreamPolicyBase.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("HTTP_StreamPolicyBase.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_STREAM_POLICY_BASE_H */
