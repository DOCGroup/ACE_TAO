/**
 * @file HTTP_URL.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_URL_H
#define ACE_HTTP_URL_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INet/INet_Export.h"
#include "ace/INet/URLBase.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_URL
        *
        * @brief Implements HTTP url support.
        *
        */
        class ACE_INET_Export URL
          : public ACE::INet::URL_INetAuthBase
          {
            public:
              URL ();
              explicit URL (ACE_CString const& url_string);
              URL (const URL& url);
              ~URL () override;

              URL& operator =(const URL& url);

              ACE_CString const& get_scheme () const override;

              ACE_CString const& get_query () const override;

              ACE_CString const& get_fragment () const override;

              virtual ACE_CString get_request_uri () const;

              void set_query (ACE_CString const& query) override;

              void set_fragment (ACE_CString const& fargment) override;

              void set_proxy (ACE_CString const& proxy_host, u_short proxy_port = HTTP_PROXY_PORT);

              ACE_CString const& get_proxy_host () const;

              u_short get_proxy_port () const;

              bool has_proxy () const;

              ACE_CString to_string () const override;

              u_short default_port () const override;

              static char const * PROTOCOL;

              static ACE_CString const& protocol ();

              enum
              {
                HTTP_PORT = 80,
                HTTP_PROXY_PORT = 8080
              };

            protected:
              explicit URL (u_short port);

              ACE::INet::ClientRequestHandler* create_default_request_handler () const override;

            private:
              ACE_CString query_;
              ACE_CString fragment_;
              ACE_CString proxy_host_;
              u_short proxy_port_;

            private:
              class Factory
                : public ACE::INet::URL_Base::Factory
                {
                  public:
                    Factory ();
                    ~Factory () override;
                    ACE_CString const& protocol () override;
                    ACE::INet::URL_Base* create_from_string (ACE_CString const& url_string) override;
                };

              using TURLFactorySingleton = ACE_Singleton<Factory, ACE_Null_Mutex>;
              static const Factory& factory_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_URL.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_URL_H */
