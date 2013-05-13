// $Id$

#include "ace/String_Base.h"
#include "ace/OS_NS_ctype.h"
#include "ace/INet/HTTP_Request.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Request.inl"
#endif

#include "ace/INet/INet_Log.h"



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    const ACE_CString Request::HTTP_GET      = "GET";
    const ACE_CString Request::HTTP_HEAD     = "HEAD";
    const ACE_CString Request::HTTP_PUT      = "PUT";
    const ACE_CString Request::HTTP_POST     = "POST";
    const ACE_CString Request::HTTP_OPTIONS  = "OPTIONS";
    const ACE_CString Request::HTTP_DELETE   = "DELETE";
    const ACE_CString Request::HTTP_TRACE    = "TRACE";
    const ACE_CString Request::HTTP_CONNECT  = "CONNECT";
    const ACE_CString Request::HOST          = "Host";
    const ACE_CString Request::COOKIE        = "Cookie";
    const ACE_CString Request::AUTHORIZATION = "Authorization";

    Request::Request()
      : method_ (HTTP_GET),
        uri_ ("/")
      {
      }

    Request::Request(const ACE_CString& version)
      : Header (version),
        method_ (HTTP_GET),
        uri_ ("/")
      {
      }

    Request::Request(const ACE_CString& method, const ACE_CString& uri)
      : method_ (method),
        uri_ (uri)
      {
      }

    Request::Request(const ACE_CString& method, const ACE_CString& uri, const ACE_CString& version)
      : Header (version),
        method_ (method),
        uri_ (uri)
      {
      }

    Request::~Request()
      {
      }

    void Request::set_host(const ACE_CString& host, u_short port)
      {
        ACE_CString val(host);
        val += ':';
        char buf[16];
        val += ACE_OS::itoa (port, buf, 10);
        this->set (HOST, val);
      }

    void Request::add_cookie(const ACE_CString & cookie)
      {
        this->add (COOKIE, cookie);
      }

    void Request::get_cookies(ACE_Array<ACE_CString> & cookies) const
      {
        this->get_values (COOKIE, cookies);
      }

    void Request::get_credentials(ACE_CString& scheme, ACE_CString& auth_info) const
      {
        if (this->has_credentials ())
        {
          ACE_CString auth;
          this->get (AUTHORIZATION, auth);
          ACE_CString::ITERATOR it (auth);
          while (!it.done () && ACE_OS::ace_isspace (*it))
            ++it;
          while (!it.done () && !ACE_OS::ace_isspace (*it))
            scheme += *it++;
          while (!it.done () && ACE_OS::ace_isspace (*it))
            ++it;
          while (!it.done ())
            auth_info += *it++;
        }
      }

    void Request::set_credentials(const ACE_CString& scheme, const ACE_CString& auth_info)
      {
        ACE_CString val (scheme);
        val += " ";
        val += auth_info;
        this->set (AUTHORIZATION, val);
      }

    void Request::write(std::ostream& str) const
      {
        str << this->method_.c_str () << " " << this->uri_.c_str () << " " << this->get_version ().c_str () << "\r\n";

        INET_DEBUG (6, (LM_DEBUG, DLINFO
                        ACE_TEXT ("ACE_INet_HTTP: --> %C %C %C\n"),
                        this->method_.c_str (),
                        this->uri_.c_str (),
                        this->get_version ().c_str ()));

        Header::write (str);
        str << "\r\n";
      }

    bool Request::read(std::istream& str)
      {
        ACE_CString method (16, '\0');
        ACE_CString uri (128, '\0');
        ACE_CString version (16, '\0');

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
        // get method
        ch = this->read_ws_field (str, method, MAX_METHOD_LENGTH);
        if (ch == eof_  || !ACE_OS::ace_isspace (ch))
          return false; // invalid HTTP method string
        // skip whitespace
        while (ACE_OS::ace_isspace (str.peek ()))
          {
            str.get ();
          }
        // get uri
        ch = this->read_ws_field (str, uri, MAX_URI_LENGTH);
        if (ch == eof_ || !ACE_OS::ace_isspace (ch))
          return false; // invalid HTTP uri string
        // skip whitespace
        while (ACE_OS::ace_isspace (str.peek ()))
          {
            str.get ();
          }
        // get version
        ch = this->read_ws_field (str, version, MAX_VERSION_LENGTH);
        if (ch == eof_ || !ACE_OS::ace_isspace (ch))
          return false; // invalid HTTP version string
        // skip to eol
        while (ch != '\n' && ch != eof_)
          ch = str.get ();
        // get header lines
        if (!Header::read (str))
          return false;
        // skip empty line
        ch = str.get ();
        while (ch != '\n' && ch != eof_)
          ch = str.get ();
        this->set_method (method);
        this->set_URI (uri);
        this->set_version(version);
        return true;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
