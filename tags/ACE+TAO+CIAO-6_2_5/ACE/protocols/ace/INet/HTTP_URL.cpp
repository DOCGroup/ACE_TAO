// $Id$

#include "ace/INet/HTTP_URL.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_URL.inl"
#endif

#include "ace/INet/String_IOStream.h"
#include "ace/INet/HTTP_ClientRequestHandler.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    const char* URL::PROTOCOL = "http";

    const ACE_CString& URL::protocol ()
      {
        static const ACE_CString protocol_ (PROTOCOL);
        return protocol_;
      }

    URL::URL ()
      : ACE::INet::URL_INetAuthBase (HTTP_PORT),
        proxy_port_ (HTTP_PROXY_PORT)
      {
      }

    URL::URL (const ACE_CString& url_string)
      : ACE::INet::URL_INetAuthBase (HTTP_PORT),
        proxy_port_ (HTTP_PROXY_PORT)
      {
        this->parse (url_string);
      }

    URL::URL (const URL& url)
      : ACE::INet::URL_INetAuthBase (0)
      {
        *this = url;
      }

    URL::URL (u_short port)
      : ACE::INet::URL_INetAuthBase (port),
        proxy_port_ (HTTP_PROXY_PORT)
      {
      }

    URL::~URL ()
      {
      }

    URL& URL::operator =(const URL& url)
      {
        this->set_user_info (url.get_user_info ());
        this->set_host (url.get_host ());
        this->set_port (url.get_port ());
        this->set_path (url.get_path ());
        this->set_query (url.get_query ());
        this->set_fragment (url.get_fragment ());
        this->set_proxy (url.get_proxy_host (), url.get_proxy_port ());
        return *this;
      }

    ACE_CString URL::get_request_uri () const
      {
        ACE::IOS::CString_OStream sos;
        if (!this->proxy_host_.empty ())
          {
            sos << this->get_scheme ().c_str () << "://"
                << ACE::INet::URL_INetBase::get_host ().c_str ();
            if (ACE::INet::URL_INetBase::get_port () != HTTP_PORT)
              {
                sos << ':' << ACE::INet::URL_INetBase::get_port ();
              }
          }
        // if path is empty we're requesting the root
        sos << (this->get_path ().empty () ?
                    "/" :
                    this->get_path ().c_str ());
        if (!this->get_query ().empty ())
          sos << '?' << this->get_query ().c_str ();
        if (!this->get_fragment ().empty ())
          sos << '#' << this->get_fragment ().c_str ();
        return sos.str ();
      }

    ACE_CString URL::to_string () const
      {
        ACE::IOS::CString_OStream sos;
        sos << this->get_scheme () << "://"
            << this->get_authority ().c_str ()
            << this->get_path ().c_str ();
        if (!this->get_query ().empty ())
          sos << '?' << this->get_query ().c_str ();
        if (!this->get_fragment ().empty ())
          sos << '#' << this->get_fragment ().c_str ();
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
        return URL::protocol ();
      }

    ACE::INet::URL_Base* URL::Factory::create_from_string (const ACE_CString& url_string)
      {
        URL* purl = 0;
        ACE_NEW_NORETURN (purl, URL (url_string));
        return purl;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
