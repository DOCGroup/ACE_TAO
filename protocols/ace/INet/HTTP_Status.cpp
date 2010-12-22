// $Id$

#include "ace/INet/HTTP_Status.h"
#include "ace/INet/HeaderBase.h"
#include "ace/OS_NS_stdlib.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Status.inl"
#endif



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    const ACE_CString Status::HTTP_REASON_CONTINUE                        = "Continue";
    const ACE_CString Status::HTTP_REASON_SWITCHING_PROTOCOLS             = "Switching Protocols";
    const ACE_CString Status::HTTP_REASON_OK                              = "OK";
    const ACE_CString Status::HTTP_REASON_CREATED                         = "Created";
    const ACE_CString Status::HTTP_REASON_ACCEPTED                        = "Accepted";
    const ACE_CString Status::HTTP_REASON_NONAUTHORITATIVE                = "Non-Authoritative Information";
    const ACE_CString Status::HTTP_REASON_NO_CONTENT                      = "No Content";
    const ACE_CString Status::HTTP_REASON_RESET_CONTENT                   = "Reset Content";
    const ACE_CString Status::HTTP_REASON_PARTIAL_CONTENT                 = "Partial Content";
    const ACE_CString Status::HTTP_REASON_MULTIPLE_CHOICES                = "Multiple Choices";
    const ACE_CString Status::HTTP_REASON_MOVED_PERMANENTLY               = "Moved Permanently";
    const ACE_CString Status::HTTP_REASON_FOUND                           = "Found";
    const ACE_CString Status::HTTP_REASON_SEE_OTHER                       = "See Other";
    const ACE_CString Status::HTTP_REASON_NOT_MODIFIED                    = "Not Modified";
    const ACE_CString Status::HTTP_REASON_USEPROXY                        = "Use Proxy";
    const ACE_CString Status::HTTP_REASON_TEMPORARY_REDIRECT              = "Temporary Redirect";
    const ACE_CString Status::HTTP_REASON_BAD_REQUEST                     = "Bad Request";
    const ACE_CString Status::HTTP_REASON_UNAUTHORIZED                    = "Unauthorized";
    const ACE_CString Status::HTTP_REASON_PAYMENT_REQUIRED                = "Payment Required";
    const ACE_CString Status::HTTP_REASON_FORBIDDEN                       = "Forbidden";
    const ACE_CString Status::HTTP_REASON_NOT_FOUND                       = "Not Found";
    const ACE_CString Status::HTTP_REASON_METHOD_NOT_ALLOWED              = "Method Not Allowed";
    const ACE_CString Status::HTTP_REASON_NOT_ACCEPTABLE                  = "Not Acceptable";
    const ACE_CString Status::HTTP_REASON_PROXY_AUTHENTICATION_REQUIRED   = "Proxy Authentication Required";
    const ACE_CString Status::HTTP_REASON_REQUEST_TIMEOUT                 = "Request Time-out";
    const ACE_CString Status::HTTP_REASON_CONFLICT                        = "Conflict";
    const ACE_CString Status::HTTP_REASON_GONE                            = "Gone";
    const ACE_CString Status::HTTP_REASON_LENGTH_REQUIRED                 = "Length Required";
    const ACE_CString Status::HTTP_REASON_PRECONDITION_FAILED             = "Precondition Failed";
    const ACE_CString Status::HTTP_REASON_REQUESTENTITYTOOLARGE           = "Request Entity Too Large";
    const ACE_CString Status::HTTP_REASON_REQUESTURITOOLONG               = "Request-URI Too Large";
    const ACE_CString Status::HTTP_REASON_UNSUPPORTEDMEDIATYPE            = "Unsupported Media Type";
    const ACE_CString Status::HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE = "Requested Range Not Satisfiable";
    const ACE_CString Status::HTTP_REASON_EXPECTATION_FAILED              = "Expectation Failed";
    const ACE_CString Status::HTTP_REASON_INTERNAL_SERVER_ERROR           = "Internal Server Error";
    const ACE_CString Status::HTTP_REASON_NOT_IMPLEMENTED                 = "Not Implemented";
    const ACE_CString Status::HTTP_REASON_BAD_GATEWAY                     = "Bad Gateway";
    const ACE_CString Status::HTTP_REASON_SERVICE_UNAVAILABLE             = "Service Unavailable";
    const ACE_CString Status::HTTP_REASON_GATEWAY_TIMEOUT                 = "Gateway Time-out";
    const ACE_CString Status::HTTP_REASON_VERSION_NOT_SUPPORTED           = "HTTP Version not supported";
    const ACE_CString Status::HTTP_REASON_UNKNOWN                         = "???";

    Status::Status()
      : code_ (HTTP_NONE)
      {
      }

    Status::Status(Code status)
      : code_ (status)
      {
        reason_ = get_reason(this->code_);
      }

    Status::Status(Code status, const ACE_CString& reason)
      : code_ (status), reason_ (reason)
      {
      }

    Status::Status(const Status& status)
      {
        *this = status;
      }

    Status& Status::operator =(const Status& status)
      {
        this->code_ = status.get_status ();
        this->reason_ = status.get_reason ();
        return *this;
      }

    void Status::set_status(const ACE_CString& status)
      {
        Code istat = (Code)ACE_OS::atoi (status.c_str ());
        if (istat == HTTP_NONE || get_reason (istat) == HTTP_REASON_UNKNOWN)
          {
            this->code_ = INVALID;
          }
        else
          {
            this->code_ = istat;
          }
      }

    const ACE_CString& Status::get_reason(Code status)
      {
        switch (status)
        {
        case HTTP_NONE:
          return ACE::INet::HeaderBase::EMPTY;
        case HTTP_CONTINUE:
          return HTTP_REASON_CONTINUE;
        case HTTP_SWITCHING_PROTOCOLS:
          return HTTP_REASON_SWITCHING_PROTOCOLS;
        case HTTP_OK:
          return HTTP_REASON_OK;
        case HTTP_CREATED:
          return HTTP_REASON_CREATED;
        case HTTP_ACCEPTED:
          return HTTP_REASON_ACCEPTED;
        case HTTP_NONAUTHORITATIVE:
          return HTTP_REASON_NONAUTHORITATIVE;
        case HTTP_NO_CONTENT:
          return HTTP_REASON_NO_CONTENT;
        case HTTP_RESET_CONTENT:
          return HTTP_REASON_RESET_CONTENT;
        case HTTP_PARTIAL_CONTENT:
          return HTTP_REASON_PARTIAL_CONTENT;
        case HTTP_MULTIPLE_CHOICES:
          return HTTP_REASON_MULTIPLE_CHOICES;
        case HTTP_MOVED_PERMANENTLY:
          return HTTP_REASON_MOVED_PERMANENTLY;
        case HTTP_FOUND:
          return HTTP_REASON_FOUND;
        case HTTP_SEE_OTHER:
          return HTTP_REASON_SEE_OTHER;
        case HTTP_NOT_MODIFIED:
          return HTTP_REASON_NOT_MODIFIED;
        case HTTP_USEPROXY:
          return HTTP_REASON_USEPROXY;
        case HTTP_TEMPORARY_REDIRECT:
          return HTTP_REASON_TEMPORARY_REDIRECT;
        case HTTP_BAD_REQUEST:
          return HTTP_REASON_BAD_REQUEST;
        case HTTP_UNAUTHORIZED:
          return HTTP_REASON_UNAUTHORIZED;
        case HTTP_PAYMENT_REQUIRED:
          return HTTP_REASON_PAYMENT_REQUIRED;
        case HTTP_FORBIDDEN:
          return HTTP_REASON_FORBIDDEN;
        case HTTP_NOT_FOUND:
          return HTTP_REASON_NOT_FOUND;
        case HTTP_METHOD_NOT_ALLOWED:
          return HTTP_REASON_METHOD_NOT_ALLOWED;
        case HTTP_NOT_ACCEPTABLE:
          return HTTP_REASON_NOT_ACCEPTABLE;
        case HTTP_PROXY_AUTHENTICATION_REQUIRED:
          return HTTP_REASON_PROXY_AUTHENTICATION_REQUIRED;
        case HTTP_REQUEST_TIMEOUT:
          return HTTP_REASON_REQUEST_TIMEOUT;
        case HTTP_CONFLICT:
          return HTTP_REASON_CONFLICT;
        case HTTP_GONE:
          return HTTP_REASON_GONE;
        case HTTP_LENGTH_REQUIRED:
          return HTTP_REASON_LENGTH_REQUIRED;
        case HTTP_PRECONDITION_FAILED:
          return HTTP_REASON_PRECONDITION_FAILED;
        case HTTP_REQUESTENTITYTOOLARGE:
          return HTTP_REASON_REQUESTENTITYTOOLARGE;
        case HTTP_REQUESTURITOOLONG:
          return HTTP_REASON_REQUESTURITOOLONG;
        case HTTP_UNSUPPORTEDMEDIATYPE:
          return HTTP_REASON_UNSUPPORTEDMEDIATYPE;
        case HTTP_REQUESTED_RANGE_NOT_SATISFIABLE:
          return HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE;
        case HTTP_EXPECTATION_FAILED:
          return HTTP_REASON_EXPECTATION_FAILED;
        case HTTP_INTERNAL_SERVER_ERROR:
          return HTTP_REASON_INTERNAL_SERVER_ERROR;
        case HTTP_NOT_IMPLEMENTED:
          return HTTP_REASON_NOT_IMPLEMENTED;
        case HTTP_BAD_GATEWAY:
          return HTTP_REASON_BAD_GATEWAY;
        case HTTP_SERVICE_UNAVAILABLE:
          return HTTP_REASON_SERVICE_UNAVAILABLE;
        case HTTP_GATEWAY_TIMEOUT:
          return HTTP_REASON_GATEWAY_TIMEOUT;
        case HTTP_VERSION_NOT_SUPPORTED:
          return HTTP_REASON_VERSION_NOT_SUPPORTED;
        default:
          return HTTP_REASON_UNKNOWN;
        }
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
