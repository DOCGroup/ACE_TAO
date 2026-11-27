/**
 * @file HTTP_Request.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_REQUEST_H
#define ACE_HTTP_REQUEST_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/Request.h"
#include "ace/INet/HTTP_Header.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        class ACE_INET_Export Request
          : public ACE::INet::Request, public Header
          {
            public:
              /// Constructor; creates a GET / HTTP/1.0 HTTP request.
              Request();

              /// Constructor; creates a GET request with given version.
              explicit Request(ACE_CString const& version);

              /// Constructor; creates a HTTP/1.0 request with given method and URI.
              Request(ACE_CString const& method, ACE_CString const& uri);

              /// Constructor; creates an HTTP request with given method, URI and version.
              Request(ACE_CString const& method, ACE_CString const& uri, ACE_CString const& version);

              ~Request () override;

              /// Reset the request object
              void reset ();

              void reset (ACE_CString const& version);

              void reset (ACE_CString const& method, ACE_CString const& uri);

              void reset (ACE_CString const& method, ACE_CString const& uri, ACE_CString const& version);

              /// Set the method.
              void set_method(ACE_CString const& method);

              /// Return the method.
              ACE_CString const& get_method() const;

              /// Set the request URI.
              void set_URI(ACE_CString const& uri);

              /// Return the request URI.
              ACE_CString const& get_URI() const;

              /// Set the Host header field.
              void set_host(ACE_CString const& host);

              void set_host(ACE_CString const& host, u_short port);

              /// Returns true if Host header field has been set.
              bool has_host () const;

              /// Returns the value of the Host header field.
              ACE_CString get_host() const;

              /// Adds a Cookie header.
              void add_cookie(ACE_CString const & cookie);

              /// Get cookies from Cookie header(s).
              void get_cookies(ACE_Array<ACE_CString> & cookies) const;

              /// Returns true if the request contains authentication
              /// information in the form of an Authorization header.
              bool has_credentials() const;

              /// Returns the authentication scheme and authentication information
              void get_credentials(ACE_CString& scheme, ACE_CString& auth_info) const;

              /// Set the authentication scheme and information
              void set_credentials(ACE_CString const& scheme, ACE_CString const& auth_info);

              /// Writes the HTTP request to the given stream
              void write (std::ostream& str) const override;

              /// Reads the HTTP request from the
              /// given stream.
              bool read (std::istream& str) override;

              static ACE_CString const HTTP_GET;
              static ACE_CString const HTTP_HEAD;
              static ACE_CString const HTTP_PUT;
              static ACE_CString const HTTP_POST;
              static ACE_CString const HTTP_OPTIONS;
              static ACE_CString const HTTP_DELETE;
              static ACE_CString const HTTP_TRACE;
              static ACE_CString const HTTP_CONNECT;

              static ACE_CString const HOST;
              static ACE_CString const COOKIE;
              static ACE_CString const AUTHORIZATION;

            private:
              enum Limits
              {
                MAX_METHOD_LENGTH  = 32,
                MAX_URI_LENGTH     = 4096,
                MAX_VERSION_LENGTH = 8
              };

              ACE_CString method_;
              ACE_CString uri_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Request.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_REQUEST_H */
