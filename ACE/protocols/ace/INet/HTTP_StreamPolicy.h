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
              using char_type = typename STREAM_BUFFER::char_type;

              FixedLengthStreamPolicyBase (std::streamsize length);
              ~FixedLengthStreamPolicyBase () override;

            protected:
              int read_from_stream (char_type* buffer, std::streamsize length) override;

              int write_to_stream (const char_type* buffer, std::streamsize length) override;

            private:
              std::streamsize length_;
              std::streamsize count_;
          };

        using FixedLengthStreamPolicy = FixedLengthStreamPolicyBase<StreamBuffer>;

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
              using char_type = typename STREAM_BUFFER::char_type;
              using buffer_type = ACE::IOS::String_OStreamBase<char_type>;

              ChunkedTransferStreamPolicyBase ();
              ~ChunkedTransferStreamPolicyBase () override;

            protected:
              int read_from_stream (char_type* buffer, std::streamsize length) override;

              int write_to_stream (const char_type* buffer, std::streamsize length) override;

              int getc ();

            private:
              std::streamsize chunk_cnt_;
              buffer_type chunk_;
          };

        using ChunkedTransferStreamPolicy = ChunkedTransferStreamPolicyBase<StreamBuffer>;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/INet/HTTP_StreamPolicy.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_STREAM_POLICY_H */
