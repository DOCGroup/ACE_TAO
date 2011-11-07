// $Id$

/**
 * @file HTTP_BasicAuthentication.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_BASIC_AUTHENTICATION_H
#define ACE_HTTP_BASIC_AUTHENTICATION_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INet/INet_Export.h"
#include "ace/INet/HTTP_Request.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_BasicAuthentication
        *
        * @brief Provides support for HTTP Basic Authentication credentials.
        *
        */
        class ACE_INET_Export BasicAuthentication
          {
            public:
              /// Constructor
              BasicAuthentication();

              /// Constructor. Sets user id and password.
              BasicAuthentication(const ACE_CString& user, const ACE_CString& passwd);

              /// Constructor. Decodes cedentials from request (if any).
              BasicAuthentication(const Request& request);

              /// Destructor
              virtual ~BasicAuthentication();

              /// Set user id.
              void user (const ACE_CString& user);

              /// Get user id.
              const ACE_CString& user () const;

              /// Set password.
              void password (const ACE_CString& passwd);

              /// Get password.
              const ACE_CString& password () const;

              /// Add authentication header with encoded credentials to request.
              void set_credentials (Request& request) const;

              static const char* SCHEME;

            private:
              // prevent copying
              BasicAuthentication(const BasicAuthentication&);
              BasicAuthentication& operator =(BasicAuthentication&);

              ACE_CString user_;
              ACE_CString passwd_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_BasicAuthentication.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_BASIC_AUTHENTICATION_H */
