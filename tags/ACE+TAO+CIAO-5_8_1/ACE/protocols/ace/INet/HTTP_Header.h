// $Id$

/**
 * @file HTTP_Header.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_HEADER_H
#define ACE_HTTP_HEADER_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INet/INet_Export.h"
#include "ace/INet/HeaderBase.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_Header
        *
        * @brief Encapsulates common HTTP message properties.
        *
        */
        class ACE_INET_Export Header
          : public ACE::INet::HeaderBase
          {
            public:
              /// Constructor
              Header();

              /// Constructor
              Header(const ACE_CString& version);

              /// Destructor
              virtual ~Header();

              /// Sets the HTTP version to use.
              void set_version(const ACE_CString& version);

              /// Returns the HTTP version.
              const ACE_CString& get_version() const;

              /// If flag is true, sets the Transfer-Encoding header to
              /// CHUNKED_TRANSFER_ENCODING. Otherwise, removes the
              /// Transfer-Encoding header.
              void set_chunked_transfer_encoding(bool flag);

              /// Returns true if the Transfer-Encoding header is set
              /// and its value is CHUNKED_TRANSFER_ENCODING.
              bool has_chunked_transfer_encoding() const;

              /// Sets the value of the Connection header field.
              /// The value is set to "Keep-Alive" if true
              /// specified, or to "Close" otherwise.
              void set_keep_alive(bool keep_alive);

              /// Returns true if
              ///   * the message has a Connection header field and its value is "Keep-Alive"
              ///   * the message is a HTTP/1.1 message and no Connection header is set
              /// Returns false otherwise.
              bool has_keep_alive() const;

              static const ACE_CString HTTP_1_0;
              static const ACE_CString HTTP_1_1;

              static const ACE_CString CHUNKED_TRANSFER_ENCODING;

              static const ACE_CString TRANSFER_ENCODING;
              static const ACE_CString CONNECTION;

              static const ACE_CString CONNECTION_KEEP_ALIVE;
              static const ACE_CString CONNECTION_CLOSE;

            private:
              ACE_CString version_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Header.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_HEADER_H */
