// $Id$

#include "ace/INet/URLBase.h"
#include "ace/INet/IOS_util.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/URLBase.inl"
#endif

#include "ace/INet/String_IOStream.h"

#include "ace/INet/ClientRequestHandler.h"
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

    bool URL_Base::parse (const ACE_CString& url_string)
      {
        static const int eof =
          std::char_traits<ACE::IOS::CString_OStream::char_type>::eof ();

        ACE_CString uri = url_string;
        if (this->strip_scheme (uri))
          {
            ACE::IOS::CString_OStream sos;
            ACE::IOS::CString_IStream sis (uri);

            int ch;

            // parse authority part (if any)
            if ((ch = this->parse_authority (sis)) == '/' ||
                !this->has_authority ())      // relative paths allowed if no authority
              {
                // parse path part
                sos.put (ch);
                for (ch = sis.get (); ch != '?' && ch != '#' && ch != eof ;ch = sis.get ())
                  sos.put (ch);
                this->set_path (sos.str ());
                sos.clear ();
              }
            else
              {
                // empty path
                this->set_path (empty_);
              }

            if (ch == '?')
              {
                // parse query part
                for (ch = sis.get (); ch != '#' && ch != eof ;ch = sis.get ())
                  sos.put (ch);
                this->set_query (sos.str ());
                sos.clear ();
              }

            if (ch == '#')
              {
                // get fragment
                sos << sis.rdbuf ();
                this->set_fragment (sos.str ());
              }
            else if (ch != eof)
              {
                // should not happen
                return false;
              }

            // check for (minimum) correctness
            return this->validate ();
          }
        return false;
      }

    int URL_Base::parse_authority(std::istream& is)
      {
        return is.get ();
      }

    bool URL_Base::has_authority ()
      {
        return false;
      }

    bool URL_Base::validate ()
      {
        return true;
      }

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

    bool URL_Base::strip_scheme (ACE_CString& url_string)
      {
        // since this will be called at a point where the
        // actual URL class is already known (and with that
        // the protocol prefix) we allow for the fact we
        // may get a url passed without the actual prefix

        ACE_CString::size_type pos = url_string.find (':');
        if (pos > 0 && url_string[pos+1] == '/' && url_string[pos+2] == '/')
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

    int URL_INetBase::parse_authority (std::istream& is)
      {
        ACE::IOS::CString_OStream sos;
        return this->parse_authority_i (is, sos, 0);
      }

    int URL_INetBase::parse_authority_i (std::istream& is,
                                         std::ostream& os,
                                         int lastch)
      {
        static const int eof =
          std::char_traits<ACE::IOS::CString_OStream::char_type>::eof ();

        ACE::IOS::CString_OStream& sos =
            dynamic_cast<ACE::IOS::CString_OStream&> (os);

        int ch = lastch;
        if (ch == 0)
          {
            // parse host part
            for (ch = is.get ();
#if defined (ACE_HAS_IPV6)
                 ch != '[' && ch != '/' && ch != ':' && ch != '@' && ch != '?' && ch != '#' && ch != eof ;
#else
                 ch != '/' && ch != ':' && ch != '@' && ch != '?' && ch != '#' && ch != eof ;
#endif
                 ch = is.get ())
              sos.put (ch);
          }

#if defined (ACE_HAS_IPV6)
        if (ch == '[')
          {
            sos.clear ();
            for (ch = is.get (); ch != ']' && ch != eof ;ch = is.get ())
              sos.put (ch);
            if (ch != eof)
              ch = is.get (); // skip ']'
            if (ch != '/' && ch != ':' && ch != '?' && ch != '#' && ch != eof)
              {
                this->set_host (empty_); // invalid URL, clear host field
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
            is >> port; // should stop at '/' or '?' or '#' or eof
            ch = is.get ();
            if (ch == '/' || ch == '?' || ch == '#' || ch == eof)
              this->set_port (port);
            else
              this->set_port (0);
          }
        else
          {
            this->set_port (this->default_port ());
          }

        return ch;
      }

    bool URL_INetBase::has_authority ()
      {
        return true;
      }

    bool URL_INetBase::validate ()
      {
        return !this->host_.empty () && this->port_>0;
      }

    ACE_CString URL_INetBase::get_authority () const
      {
        ACE::IOS::CString_OStream sos;
        sos << this->get_host().c_str ();
        if (this->get_port () != this->default_port ())
          sos << ':' << this->get_port ();
        return sos.str ();
      }

    URL_INetAuthBase::authenticator_map URL_INetAuthBase::authenticators_;

    URL_INetAuthBase::URL_INetAuthBase (u_short port)
      : URL_INetBase (port)
      {
      }

    URL_INetAuthBase::~URL_INetAuthBase () {}

    ACE_CString URL_INetAuthBase::get_authority () const
      {
        ACE::IOS::CString_OStream sos;
        if (!this->get_user_info ().empty ())
          sos << this->get_user_info ().c_str () << "@";
        sos << this->get_host().c_str ();
        if (this->get_port () != this->default_port ())
          sos << ':' << this->get_port ();
        return sos.str ();
      }

    int URL_INetAuthBase::parse_authority (std::istream& is)
      {
        static const int eof =
          std::char_traits<ACE::IOS::CString_OStream::char_type>::eof ();

        ACE::IOS::CString_OStream sos;

        int ch;
        // parse userinfo (if any)
        for (ch = is.get ();
#if defined (ACE_HAS_IPV6)
             ch != '[' && ch != '/' && ch != ':' && ch != '@' && ch != '?' && ch != '#' && ch != eof ;
#else
             ch != '/' && ch != ':' && ch != '@' && ch != '?' && ch != '#' && ch != eof ;
#endif
             ch = is.get ())
          sos.put (ch);

        if (ch == '@')
          {
            this->set_user_info (sos.str ());
            sos.clear ();
            ch = URL_INetBase::parse_authority_i (is, sos, 0);
          }
        else
          {
            ch = URL_INetBase::parse_authority_i (is, sos, ch);
          }

        return ch;
      }

    bool URL_INetAuthBase::add_authenticator (const ACE_CString& auth_id,
                                             AuthenticatorBase* authenticator)
      {
        if (URL_INetAuthBase::authenticators_.find (auth_id) == -1)
          {
            return URL_INetAuthBase::authenticators_.bind (auth_id,
                                                           authenticator_ptr (authenticator)) == 0;
          }
        return false;
      }

    bool URL_INetAuthBase::has_authenticator (const ACE_CString& auth_id)
      {
        return (URL_INetAuthBase::authenticators_.find (auth_id) == 0);
      }

    AuthenticatorBase* URL_INetAuthBase::remove_authenticator (const ACE_CString& auth_id)
      {
        authenticator_ptr auth;
        if (URL_INetAuthBase::authenticators_.unbind (auth_id, auth) == 0)
          {
            auth.release ();
          }
        return 0;
      }

    bool URL_INetAuthBase::authenticate (AuthenticationBase& authentication)
      {
        ACE_GUARD_RETURN (ACE_SYNCH::RECURSIVE_MUTEX,
                          _guard,
                          URL_INetAuthBase::authenticators_.mutex (),
                          false);

        authenticator_map::iterator it = URL_INetAuthBase::authenticators_.begin ();
        for (; it != URL_INetAuthBase::authenticators_.end ();
             ++it)
          {
            authenticator_ptr auth_ptr = (*it).int_id_;

            // release lock before calling user code
            if (URL_INetAuthBase::authenticators_.mutex ().release () != 0)
              return false;

            if (auth_ptr->authenticate (authentication))
              return true;

            // re-acquire lock
            if (URL_INetAuthBase::authenticators_.mutex ().acquire () != 0)
              return false;
          }

        return false;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
