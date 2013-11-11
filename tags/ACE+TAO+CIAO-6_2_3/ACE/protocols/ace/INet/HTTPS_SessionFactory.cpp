// $Id$

#include "ace/INet/HTTPS_SessionFactory.h"

#include "ace/INet/INet_Log.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTPS
  {

    SessionFactory_Impl::SessionHolder_Impl::SessionHolder_Impl ()
      : session_ (true)
      {
      }

    SessionFactory_Impl::SessionHolder_Impl::~SessionHolder_Impl()
      {
      }

    ACE::HTTP::SessionBase& SessionFactory_Impl::SessionHolder_Impl::session ()
      {
        return this->session_;
      }

    SessionFactory_Impl& SessionFactory_Impl::factory_ =
        *ACE_Singleton<SessionFactory_Impl,ACE_SYNCH::NULL_MUTEX>::instance ();

    SessionFactory_Impl::SessionFactory_Impl ()
      {
        INET_DEBUG (6, (LM_INFO, DLINFO
                                 ACE_TEXT ("HTTPS_SessionFactory_Impl::ctor - ")
                                 ACE_TEXT ("registering session factory for scheme [%C]\n"),
                                 URL::protocol ().c_str ()));
        ACE::HTTP::SessionFactoryRegistry::instance ().register_session_factory (URL::protocol (), this);
      }

    SessionFactory_Impl::~SessionFactory_Impl ()
      {
      }

    ACE::INet::ConnectionHolder*
    SessionFactory_Impl::create_connection (
        const ACE::INet::ConnectionKey& key) const
      {
        INET_TRACE ("HTTPS_SessionFactory_Impl::create_connection");

        const ACE::HTTP::ClientRequestHandler::HttpConnectionKey& ikey =
            dynamic_cast<const ACE::HTTP::ClientRequestHandler::HttpConnectionKey&> (key);

        SessionHolder_Impl* session_holder = 0;
        ACE_NEW_RETURN (session_holder,
                        SessionHolder_Impl (),
                        0);
        ACE_Auto_Ptr<SessionHolder_Impl> session_safe_ref (session_holder);

        (*session_holder)->set_host (ikey.host (), ikey.port ());
        if (ikey.is_proxy_connection ())
          {
            (*session_holder)->set_proxy_target (ikey.proxy_target_host (),
                                                 ikey.proxy_target_port ());
          }

        if ((*session_holder)->connect (true))
          {
            return session_safe_ref.release ();
          }

        return 0;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
