// $Id$

#include "HTTP_URL.h"

#if !defined (__ACE_INLINE__)
#include "HTTP_URL.inl"
#endif

#include "String_IOStream.h"
#include "HTTP_ClientRequestHandler.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    const ACE_CString URL::PROTOCOL ("http");

    URL::URL ()
      : URL_INetBase (HTTP_PORT),
        proxy_port_ (HTTP_PROXY_PORT)
      {
      }

    URL::URL (const ACE_CString& url_string)
      : URL_INetBase (HTTP_PORT),
        proxy_port_ (HTTP_PROXY_PORT)
      {
        this->parse (url_string);
      }

    URL::URL (const URL& url)
      : URL_INetBase (0)
      {
        *this = url;
      }

    URL::~URL ()
      {
      }

    URL& URL::operator =(const URL& url)
      {
        URL_INetBase::set_port (url.get_port ());
        this->set_host (url.get_host ());
        this->set_user_info (url.get_user_info ());
        this->set_path (url.get_path ());
        this->set_query (url.get_query ());
        this->set_proxy (url.get_proxy_host (), url.get_proxy_port ());
        return *this;
      }

    bool URL::parse (const ACE_CString& url_string)
      {
        static const int eof =
          std::char_traits<ACE::IOS::CString_OStream::char_type>::eof ();

        ACE_CString uri = url_string;
        if (this->strip_protocol (uri))
          {
            ACE::IOS::CString_OStream sos;
            ACE::IOS::CString_IStream sis (uri);

            int ch;
            for (ch = sis.get ();
#if defined (ACE_HAS_IPV6)
                 ch != '[' && ch != '/' && ch != ':' && ch != '@' && ch != '?' && ch != eof ;
#else
                 ch != '/' && ch != ':' && ch != '@' && ch != '?' && ch != eof ;
#endif
                 ch = sis.get ())
              sos.put (ch);

            if (ch == '@')
              {
                this->set_user_info (sos.str ());
                sos.clear ();

                for (ch = sis.get ();
#if defined (ACE_HAS_IPV6)
                     ch != '[' && ch != '/' && ch != ':' && ch != '?' && ch != eof ;
#else
                     ch != '/' && ch != ':' && ch != '?' && ch != eof ;
#endif
                     ch = sis.get ())
                  sos.put (ch);
              }

#if defined (ACE_HAS_IPV6)
            if (ch == '[')
              {
                sos.clear ();
                for (ch = sis.get (); ch != ']' && ch != eof ;ch = sis.get ())
                  sos.put (ch);
                if (ch != eof)
                  ch = sis.get (); // skip ']'
                if (ch != '/' && ch != ':' && ch != '?' && ch != eof)
                  {
                    this-.set_host (empty_); // invalid URL, clear host field
                    ch = eof; // stop parsing
                  }
                else
                  {
                    this->set_host (sos.str ());
                  }
              }
            else
              {
#endif
            this->set_host (sos.str ());
#if defined (ACE_HAS_IPV6)
              }
#endif
            sos.clear ();

            if (ch == ':')
              {
                u_short port = 0;
                sis >> port; // should stop at '/' or '?' or eof
                this->set_port (port);
                ch = sis.get ();
              }

            if (ch == '/')
              {
                sos.put (ch);
                for (ch = sis.get (); ch != '?' && ch != eof ;ch = sis.get ())
                  sos.put (ch);
                this->set_path (sos.str ());
                sos.clear ();
              }
            else
              {
                // default path when not specified in URL
                this->set_path ('/');
              }

            if (ch == '?')
              {
                sos << sis.rdbuf ();
                this->set_query (sos.str ());
              }
            else if (ch != eof)
              {
                // something went wrong at port extraction
                this->set_port (0);
              }

            // check for host + port + path minimum correctness
            if (!this->get_host ().empty () &&
                  this->get_port () != 0 &&
                  !this->get_path ().empty () &&
                  this->get_path ()[0] == '/')
              {
                return true;
              }
          }
        return false;
      }

    ACE_CString URL::get_authority () const
      {
        ACE::IOS::CString_OStream sos;
        sos << this->get_protocol ().c_str () << "://";
        if (!this->get_user_info ().empty ())
          sos << this->get_user_info ().c_str () << "@";
        sos << this->get_host().c_str ();
        if (this->get_port () != HTTP_PORT)
          sos << ':' << this->get_port ();
        return sos.str ();
      }

    ACE_CString URL::get_request_uri () const
      {
        ACE::IOS::CString_OStream sos;
        if (!this->proxy_host_.empty ())
          {
            sos << this->get_protocol ().c_str () << "://"
                << ACE::INet::URL_INetBase::get_host ().c_str ();
            if (ACE::INet::URL_INetBase::get_port () != HTTP_PORT)
              {
                sos << ':' << ACE::INet::URL_INetBase::get_port ();
              }
          }
        sos << this->get_path ().c_str ();
        if (!this->get_query ().empty ())
          sos << '?' << this->get_query ().c_str ();
        return sos.str ();
      }

    ACE_CString URL::to_string () const
      {
        ACE::IOS::CString_OStream sos;
        sos << this->get_authority ().c_str ()
            << this->get_path ().c_str ();
        if (!this->get_query ().empty ())
          sos << '?' << this->get_query ().c_str ();
        return sos.str ();
      }

    ACE::INet::ClientRequestHandler* URL::create_default_request_handler () const
      {
        ACE::INet::ClientRequestHandler* prh = 0;
        ACE_NEW_NORETURN (prh, ClientRequestHandler ());
        return prh;
      }

    const URL::Factory& URL::factory_ = *URL::TURLFactorySingleton::instance ();

    URL::Factory::Factory ()
      {
        ACE::INet::URL_Base::register_factory (this);
      }

    URL::Factory::~Factory ()
      {}

    const ACE_CString& URL::Factory::protocol ()
      {
        return URL::PROTOCOL;
      }

    ACE::INet::URL_Base* URL::Factory::create_from_string (const ACE_CString& url_string)
      {
        URL* purl = 0;
        ACE_NEW_NORETURN (purl, URL (url_string));
        return purl;
      }
  };
};

ACE_END_VERSIONED_NAMESPACE_DECL
