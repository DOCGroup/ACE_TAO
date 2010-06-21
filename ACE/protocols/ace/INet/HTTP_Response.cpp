// $Id$

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_ctype.h"
#include "ace/String_Base.h"
#include "ace/INet/HTTP_Response.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Response.inl"
#endif

ACE_RCSID(NET_CLIENT,ACE_HTTP_Response,"$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    const ACE_CString Response::COOKIE = "Set-Cookie";

    Response::Response()
      {
      }

    Response::Response(const Status& status)
      : status_ (status)
      {
      }

    Response::Response(const ACE_CString& version, const Status& status)
      : Header (version), status_ (status)
      {
      }

    Response::~Response()
      {
      }

    void Response::add_cookie(const ACE_CString & cookie)
      {
        this->add (COOKIE, cookie);
      }

    void Response::get_cookies(ACE_Array<ACE_CString> & cookies) const
      {
        this->get_values (COOKIE, cookies);
      }

    void Response::write(ostream& str) const
      {
        str << this->get_version ().c_str () << " "
            << static_cast<int>(this->status_.get_status ()) << " "
            << this->status_.get_reason ().c_str () << "\r\n";
        Header::write (str);
        str << "\r\n";
      }

    bool Response::read(istream& str)
      {
        ACE_CString version;
        ACE_CString status;
        ACE_CString reason;

        int ch =  str.peek ();
        if (ch == eof_)
          {
            str.get (); // skip to eof
            return false;
          }
        // skip whitespace
        while (ACE_OS::ace_isspace (str.peek ()))
          {
            str.get ();
          }
        // get version
        ch = this->read_ws_field (str, version, MAX_VERSION_LENGTH);
        if (ch == eof_  || !ACE_OS::ace_isspace (ch))
          return false; // invalid HTTP version string
        // skip whitespace
        while (ACE_OS::ace_isspace (str.peek ()))
          {
            str.get ();
          }
        // get status
        ch = this->read_ws_field (str, status, MAX_STATUS_LENGTH);
        if (ch == eof_ || !ACE_OS::ace_isspace (ch))
          return false; // invalid HTTP status code
        // skip whitespace
        while (ACE_OS::ace_isspace (str.peek ()))
          {
            str.get ();
          }
        // get reason
        ch = this->read_field (str, reason, MAX_REASON_LENGTH, '\r');
        if (ch == '\r')
          ch = str.get (); // get lf
        if (ch != '\n')
          return false; // HTTP reason string too long
        // get header lines
        if (!Header::read (str))
          return false;
        // skip empty line
        ch = str.get ();
        while (ch != '\n' && ch != eof_)
          ch = str.get ();
        this->set_version(version);
        this->status_.set_status (status);
        this->status_.set_reason (reason);
        return true;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
