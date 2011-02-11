// $Id$

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
              Request(const ACE_CString& version);

              /// Constructor; creates a HTTP/1.0 request with given method and URI.
              Request(const ACE_CString& method, const ACE_CString& uri);

              /// Constructor; creates an HTTP request with given method, URI and version.
              Request(const ACE_CString& method, const ACE_CString& uri, const ACE_CString& version);

              /// Destructor
              virtual ~Request();

              /// Reset the request object
              void reset ();

              void reset (const ACE_CString& version);

              void reset (const ACE_CString& method, const ACE_CString& uri);

              void reset (const ACE_CString& method, const ACE_CString& uri, const ACE_CString& version);

              /// Set the method.
              void set_method(const ACE_CString& method);

              /// Return the method.
              const ACE_CString& get_method() const;

              /// Set the request URI.
              void set_URI(const ACE_CString& uri);

              /// Return the request URI.
              const ACE_CString& get_URI() const;

              /// Set the Host header field.
              void set_host(const ACE_CString& host);

              void set_host(const ACE_CString& host, u_short port);

              /// Returns true if Host header field has been set.
              bool has_host () const;

              /// Returns the value of the Host header field.
              ACE_CString get_host() const;

              /// Adds a Cookie header.
              void add_cookie(const ACE_CString & cookie);

              /// Get cookies from Cookie header(s).
              void get_cookies(ACE_Array<ACE_CString> & cookies) const;

              /// Returns true if the request contains authentication
              /// information in the form of an Authorization header.
              bool has_credentials() const;

              /// Returns the authentication scheme and authentication information
              void get_credentials(ACE_CString& scheme, ACE_CString& auth_info) const;

              /// Set the authentication scheme and information
              void set_credentials(const ACE_CString& scheme, const ACE_CString& auth_info);

              /// Writes the HTTP request to the given stream
              void write(std::ostream& str) const;

              /// Reads the HTTP request from the
              /// given stream.
              bool read(std::istream& str);

              static const ACE_CString HTTP_GET;
              static const ACE_CString HTTP_HEAD;
              static const ACE_CString HTTP_PUT;
              static const ACE_CString HTTP_POST;
              static const ACE_CString HTTP_OPTIONS;
              static const ACE_CString HTTP_DELETE;
              static const ACE_CString HTTP_TRACE;
              static const ACE_CString HTTP_CONNECT;

              static const ACE_CString HOST;
              static const ACE_CString COOKIE;
              static const ACE_CString AUTHORIZATION;

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
