#include "ace/INet/FTP_URL.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/FTP_URL.inl"
#endif

#include "ace/INet/String_IOStream.h"
#include "ace/INet/FTP_ClientRequestHandler.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {
    char const * URL::PROTOCOL = "ftp";

    ACE_CString const& URL::protocol ()
      {
        static ACE_CString const protocol_ (PROTOCOL);
        return protocol_;
      }

    URL::URL ()
      : ACE::INet::URL_INetAuthBase (FTP_PORT)
      {
      }

    URL::URL (ACE_CString const& url_string)
      : ACE::INet::URL_INetAuthBase (FTP_PORT)
      {
        this->parse (url_string);
      }

    URL::URL (URL const& url)
      : ACE::INet::URL_INetAuthBase (0)
      {
        *this = url;
      }

    URL::~URL () = default;

    URL& URL::operator =(URL const& url)
      {
        this->set_user_info (url.get_user_info ());
        this->set_host (url.get_host ());
        this->set_port (url.get_port ());
        this->set_path (url.get_path ());
        return *this;
      }

    ACE_CString URL::to_string () const
      {
        ACE::IOS::CString_OStream sos;
        sos << this->get_scheme () << "://"
            << this->get_authority ().c_str ()
            << this->get_path ().c_str ();
        return sos.str ();
      }

    ACE::INet::ClientRequestHandler* URL::create_default_request_handler () const
      {
        ACE::INet::ClientRequestHandler* prh {};
        ACE_NEW_NORETURN (prh, ClientRequestHandler ());
        return prh;
      }

    URL::Factory const& URL::factory_ = *URL::TURLFactorySingleton::instance ();

    URL::Factory::Factory ()
      {
        ACE::INet::URL_Base::register_factory (this);
      }

    URL::Factory::~Factory () = default;

    ACE_CString const& URL::Factory::protocol ()
      {
        return URL::protocol ();
      }

    ACE::INet::URL_Base* URL::Factory::create_from_string (ACE_CString const& url_string)
      {
        URL* purl {};
        ACE_NEW_NORETURN (purl, URL (url_string));
        return purl;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
