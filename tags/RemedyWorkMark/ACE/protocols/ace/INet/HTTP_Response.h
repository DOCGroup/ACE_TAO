// $Id$

/**
 * @file HTTP_Response.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_RESPONSE_H
#define ACE_HTTP_RESPONSE_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/Response.h"
#include "ace/INet/HTTP_Header.h"
#include "ace/INet/HTTP_Status.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_Response
        *
        * @brief Encapsulates HTTP response message properties.
        *
        */
        class ACE_INET_Export Response
          : public ACE::INet::Response, public Header
          {
            public:
              /// Constructor
              Response();

              Response(const Status& status);

              Response(const ACE_CString& version, const Status& status = Status());

              /// Destructor
              virtual ~Response();

              /// Reset the response object
              void reset ();

              void reset (const Status& status);

              void reset (const ACE_CString& version, const Status& status = Status());

              /// Set the status.
              void set_status(const Status& status);

              /// Return the status.
              const Status& get_status() const;

              /// Adds a Cookie header.
              void add_cookie(const ACE_CString& cookie);

              /// Get cookies from Cookie header(s)).
              void get_cookies(ACE_Array<ACE_CString> & cookies) const;

              /// Writes the HTTP response to the given stream
              virtual void write(std::ostream& str) const;

              /// Reads the HTTP response from the
              /// given stream.
              /// 100 Continue responses are ignored.
              virtual bool read(std::istream& str);

              static const ACE_CString COOKIE;

            private:
              enum Limits
              {
                MAX_VERSION_LENGTH = 8,
                MAX_STATUS_LENGTH  = 3,
                MAX_REASON_LENGTH  = 512
              };

              Status  status_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Response.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_RESPONSE_H */
