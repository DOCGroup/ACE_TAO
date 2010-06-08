// $Id$

#include "URLBase.h"
#include "IOS_util.h"

#if !defined (__ACE_INLINE__)
#include "URLBase.inl"
#endif

#include "ClientRequestHandler.h"
#include <istream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    URLStream::URLStream (const URLStream& url_stream)
      : request_handler_ref_ (url_stream.request_handler_ref_),
        request_handler_ (url_stream.request_handler_)
      {
      }

    URLStream::~URLStream ()
      {
      }

    bool URLStream::operator ! ()
      {
        return this->request_handler_ == 0 || !this->request_handler_->is_response_ok ();
      }

    URLStream::operator bool ()
      {
        return this->request_handler_ != 0 && this->request_handler_->is_response_ok ();
      }

    std::istream& URLStream::operator * ()
      {
        return this->request_handler_ ?
          this->request_handler_->response_stream () :
          ACE::IOS::Null::in_stream_;
      }

    std::istream* URLStream::operator -> ()
      {
        return this->request_handler_ ?
          &this->request_handler_->response_stream () :
          &ACE::IOS::Null::in_stream_;
      }

    URLStream::URLStream (ClientRequestHandler& rh)
      : request_handler_ (&rh)
      {
      }

    URLStream::URLStream (ClientRequestHandler* rh)
      : request_handler_ref_ (rh),
        request_handler_ (rh)
      {
      }

    const ACE_CString URL_Base::empty_;

    URL_Base::URL_Base ()
      {
      }

    URL_Base::~URL_Base () {}

    URLStream URL_Base::open () const
      {
        ClientRequestHandler* rh = this->create_default_request_handler ();
        if (rh)
          {
            rh->handle_open_request (*this);
            return URLStream (rh);
          }
        else
          return URLStream (0);
      }

    URLStream URL_Base::open (ClientRequestHandler& rh) const
      {
        rh.handle_open_request (*this);
        return URLStream (rh);
      }

    URL_Base* URL_Base::create_from_string (const ACE_CString& url_string)
      {
        ACE_CString::size_type pos = url_string.find (':');
        if (pos >0 )
          {
            Factory* url_factory = 0;
            if (factories_->find (url_string.substr (0, pos), url_factory) == 0)
              {
                return url_factory->create_from_string (url_string);
              }
          }

        return 0;
      }

#if defined (ACE_HAS_WCHAR)
    bool URL_Base::parse (const ACE_WString& url_string)
      {
        return this->parse (ACE_Wide_To_Ascii (url_string.c_str ()).char_rep ());
      }

    ACE_WString URL_Base::to_wstring () const
      {
        return ACE_Ascii_To_Wide (this->to_string().c_str ()).wchar_rep ();
      }

    URL_Base* URL_Base::create_from_wstring (const ACE_WString& url_string)
      {
        return create_from_string (ACE_Wide_To_Ascii (url_string.c_str ()).char_rep ());
      }
#endif

    bool URL_Base::strip_protocol (ACE_CString& url_string)
      {
        // since this will be called at a point where the
        // actual URL class is already known (and with that
        // the protocol prefix) we allow for the fact we
        // may get a url passed without the actual prefix

        ACE_CString::size_type pos = url_string.find (':');
        if (pos > 0 && url_string[pos+1] == '/' && url_string[pos+1] == '/')
          {
            // in case we find a scheme check for the right protocol
            if (this->get_protocol () != url_string.substr (0, pos))
              {
                return false;
              }
            url_string = url_string.substr (pos+3); // skip '<protocol>://'
          }
        return true;
      }

    void URL_Base::register_factory (Factory* url_factory)
      {
        if (factories_ == 0)
          {
            factories_ = URL_Base::TURLFactorySingleton::instance ();
          }
        if (url_factory)
          factories_->bind (url_factory->protocol (), url_factory);
      }

    void URL_Base::deregister_factory (Factory* url_factory)
      {
        if (factories_ && url_factory)
          {
            factories_->unbind (url_factory->protocol ());
          };
      }

    URL_Base::TURLFactoryMap* URL_Base::factories_ = 0;

    URL_Base::Factory::Factory ()
      {}

    URL_Base::Factory::~Factory ()
      {}

    URL_INetBase::URL_INetBase (u_short port)
      : URL_Base (), port_ (port)
      {
      }

    URL_INetBase::~URL_INetBase () {}

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
