// $Id$

/**
 * @file HTTP_StreamPolicy.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_STREAM_POLICY_H
#define ACE_HTTP_STREAM_POLICY_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"
#include "ace/INet/HTTP_StreamPolicyBase.h"
#include "ace/INet/HTTP_IOStream.h"
#include "ace/INet/String_IOStream.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_FixedLengthStreamPolicy
        *
        * @brief Implements stream policy for fixed length data stream
        *   in HTTP response or request.
        *
        */
        template <class STREAM_BUFFER>
        class FixedLengthStreamPolicyBase
          : public StreamPolicyBase<STREAM_BUFFER>
          {
            public:
              typedef typename STREAM_BUFFER::char_type char_type;

              FixedLengthStreamPolicyBase (std::streamsize length);
              virtual ~FixedLengthStreamPolicyBase ();

            protected:
              virtual int read_from_stream (char_type* buffer, std::streamsize length);

              virtual int write_to_stream (const char_type* buffer, std::streamsize length);

            private:
              std::streamsize length_;
              std::streamsize count_;
          };

        typedef FixedLengthStreamPolicyBase<StreamBuffer> FixedLengthStreamPolicy;

        /**
        * @class ACE_HTTP_ChunkedTransferStreamPolicy
        *
        * @brief Implements stream policy for chunked data streams
        *   in HTTP response or request.
        *
        */
        template <class STREAM_BUFFER>
        class ChunkedTransferStreamPolicyBase
          : public StreamPolicyBase<STREAM_BUFFER>
          {
            public:
              typedef typename STREAM_BUFFER::char_type char_type;
              typedef ACE::IOS::String_OStreamBase<char_type> buffer_type;

              ChunkedTransferStreamPolicyBase ();
              virtual ~ChunkedTransferStreamPolicyBase ();

            protected:
              virtual int read_from_stream (char_type* buffer, std::streamsize length);

              virtual int write_to_stream (const char_type* buffer, std::streamsize length);

              int getc ();

            private:
              std::streamsize chunk_cnt_;
              buffer_type chunk_;
          };

        typedef ChunkedTransferStreamPolicyBase<StreamBuffer> ChunkedTransferStreamPolicy;

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/HTTP_StreamPolicy.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("HTTP_StreamPolicy.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_STREAM_POLICY_H */
