// $Id$

/**
 * @file HTTPS_SessionFactory.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTPS_SESSION_FACTORY_H
#define ACE_HTTPS_SESSION_FACTORY_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INet/HTTP_ClientRequestHandler.h"
#include "ace/INet/HTTPS_Session.h"
#include "ace/INet/INet_SSL_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTPS
      {
        /**
        * @class ACE_HTTPS_SessionFactory_Impl
        *
        * @brief Implements HTTPS session factory.
        *
        */
        class ACE_INET_SSL_Export SessionFactory_Impl
          : public ACE::HTTP::SessionFactory
          {
            private:
              SessionFactory_Impl ();
              virtual ~SessionFactory_Impl ();

              friend class ACE_Singleton<SessionFactory_Impl, ACE_SYNCH::NULL_MUTEX>;

              static SessionFactory_Impl& factory_;

              class SessionHolder_Impl : public ACE::HTTP::SessionHolder
                {
                  public:
                    SessionHolder_Impl ();
                    virtual ~SessionHolder_Impl();

                  protected:
                    virtual ACE::HTTP::SessionBase& session ();

                  private:
                    Session_T<ACE_SYNCH> session_;
                };

            public:
              virtual ACE::INet::ConnectionHolder* create_connection (
                  const ACE::INet::ConnectionKey& key) const;
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_HTTPS_SESSION_FACTORY_H */
