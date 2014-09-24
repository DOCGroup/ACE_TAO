// $Id$

/**
 * @file HTTP_Status.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_STATUS_H
#define ACE_HTTP_STATUS_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "ace/INet/INet_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_Status
        *
        * @brief Encapsulates HTTP status codes and reasons.
        *
        */
        class ACE_INET_Export Status
          {
            public:
              enum Code
                {
                  INVALID                              = -1,
                  HTTP_NONE                            = 0,
                  HTTP_CONTINUE                        = 100,
                  HTTP_SWITCHING_PROTOCOLS             = 101,
                  HTTP_OK                              = 200,
                  HTTP_CREATED                         = 201,
                  HTTP_ACCEPTED                        = 202,
                  HTTP_NONAUTHORITATIVE                = 203,
                  HTTP_NO_CONTENT                      = 204,
                  HTTP_RESET_CONTENT                   = 205,
                  HTTP_PARTIAL_CONTENT                 = 206,
                  HTTP_MULTIPLE_CHOICES                = 300,
                  HTTP_MOVED_PERMANENTLY               = 301,
                  HTTP_FOUND                           = 302,
                  HTTP_SEE_OTHER                       = 303,
                  HTTP_NOT_MODIFIED                    = 304,
                  HTTP_USEPROXY                        = 305,
                  // 306 not defined
                  HTTP_TEMPORARY_REDIRECT              = 307,
                  HTTP_BAD_REQUEST                     = 400,
                  HTTP_UNAUTHORIZED                    = 401,
                  HTTP_PAYMENT_REQUIRED                = 402,
                  HTTP_FORBIDDEN                       = 403,
                  HTTP_NOT_FOUND                       = 404,
                  HTTP_METHOD_NOT_ALLOWED              = 405,
                  HTTP_NOT_ACCEPTABLE                  = 406,
                  HTTP_PROXY_AUTHENTICATION_REQUIRED   = 407,
                  HTTP_REQUEST_TIMEOUT                 = 408,
                  HTTP_CONFLICT                        = 409,
                  HTTP_GONE                            = 410,
                  HTTP_LENGTH_REQUIRED                 = 411,
                  HTTP_PRECONDITION_FAILED             = 412,
                  HTTP_REQUESTENTITYTOOLARGE           = 413,
                  HTTP_REQUESTURITOOLONG               = 414,
                  HTTP_UNSUPPORTEDMEDIATYPE            = 415,
                  HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
                  HTTP_EXPECTATION_FAILED              = 417,
                  HTTP_INTERNAL_SERVER_ERROR           = 500,
                  HTTP_NOT_IMPLEMENTED                 = 501,
                  HTTP_BAD_GATEWAY                     = 502,
                  HTTP_SERVICE_UNAVAILABLE             = 503,
                  HTTP_GATEWAY_TIMEOUT                 = 504,
                  HTTP_VERSION_NOT_SUPPORTED           = 505
                };

              /// Constructor; sets value to OK
              Status();

              /// Constructor; sets corresponding reason
              Status(Code status);

              /// Constructor
              Status(Code status, const ACE_CString& reason);

              /// Copy constructor
              Status(const Status& status);

              /// Copy assignment
              Status& operator =(const Status& status);

              /// Set the status code.
              void set_status(Code status);

              /// Set the status code.
              /// String must contain valid numeric code.
              void set_status(const ACE_CString& status);

              /// Return the status code.
              Code get_status() const;

              /// Set the reason phrase.
              void set_reason(const ACE_CString& reason);

              /// Return the reason phrase.
              const ACE_CString& get_reason() const;

              /// Set the status code and corresponding reason phrase.
              void set_status_and_reason(Code status);

              /// Return true if status code != INVALID
              bool is_valid () const;

              /// Return true if status code == OK
              bool is_ok () const;

              /// Return true in case of a *valid* HTTP status
              operator bool () const;

              /// Return true if status code == INVALID
              bool operator !() const;

              /// Returns an appropriate reason phrase
              static const ACE_CString& get_reason(Code status);

              static const ACE_CString HTTP_REASON_CONTINUE;
              static const ACE_CString HTTP_REASON_SWITCHING_PROTOCOLS;
              static const ACE_CString HTTP_REASON_OK;
              static const ACE_CString HTTP_REASON_CREATED;
              static const ACE_CString HTTP_REASON_ACCEPTED;
              static const ACE_CString HTTP_REASON_NONAUTHORITATIVE;
              static const ACE_CString HTTP_REASON_NO_CONTENT;
              static const ACE_CString HTTP_REASON_RESET_CONTENT;
              static const ACE_CString HTTP_REASON_PARTIAL_CONTENT;
              static const ACE_CString HTTP_REASON_MULTIPLE_CHOICES;
              static const ACE_CString HTTP_REASON_MOVED_PERMANENTLY;
              static const ACE_CString HTTP_REASON_FOUND;
              static const ACE_CString HTTP_REASON_SEE_OTHER;
              static const ACE_CString HTTP_REASON_NOT_MODIFIED;
              static const ACE_CString HTTP_REASON_USEPROXY;
              static const ACE_CString HTTP_REASON_TEMPORARY_REDIRECT;
              static const ACE_CString HTTP_REASON_BAD_REQUEST;
              static const ACE_CString HTTP_REASON_UNAUTHORIZED;
              static const ACE_CString HTTP_REASON_PAYMENT_REQUIRED;
              static const ACE_CString HTTP_REASON_FORBIDDEN;
              static const ACE_CString HTTP_REASON_NOT_FOUND;
              static const ACE_CString HTTP_REASON_METHOD_NOT_ALLOWED;
              static const ACE_CString HTTP_REASON_NOT_ACCEPTABLE;
              static const ACE_CString HTTP_REASON_PROXY_AUTHENTICATION_REQUIRED;
              static const ACE_CString HTTP_REASON_REQUEST_TIMEOUT;
              static const ACE_CString HTTP_REASON_CONFLICT;
              static const ACE_CString HTTP_REASON_GONE;
              static const ACE_CString HTTP_REASON_LENGTH_REQUIRED;
              static const ACE_CString HTTP_REASON_PRECONDITION_FAILED;
              static const ACE_CString HTTP_REASON_REQUESTENTITYTOOLARGE;
              static const ACE_CString HTTP_REASON_REQUESTURITOOLONG;
              static const ACE_CString HTTP_REASON_UNSUPPORTEDMEDIATYPE;
              static const ACE_CString HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE;
              static const ACE_CString HTTP_REASON_EXPECTATION_FAILED;
              static const ACE_CString HTTP_REASON_INTERNAL_SERVER_ERROR;
              static const ACE_CString HTTP_REASON_NOT_IMPLEMENTED;
              static const ACE_CString HTTP_REASON_BAD_GATEWAY;
              static const ACE_CString HTTP_REASON_SERVICE_UNAVAILABLE;
              static const ACE_CString HTTP_REASON_GATEWAY_TIMEOUT;
              static const ACE_CString HTTP_REASON_VERSION_NOT_SUPPORTED;
              static const ACE_CString HTTP_REASON_UNKNOWN;

            private:
              Code  code_;
              ACE_CString reason_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Status.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_STATUS_H */
