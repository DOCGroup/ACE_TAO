/**
 * @file FTP_URL.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_FTP_URL_H
#define ACE_FTP_URL_H

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
    namespace FTP
      {
        /**
        * @class ACE_FTP_URL
        *
        * @brief
        *
        */
        class ACE_INET_Export URL
          : public ACE::INet::URL_INetAuthBase
          {
            public:
              URL ();
              explicit URL (ACE_CString const& url_string);
              URL (URL const& url);
              ~URL () override;

              URL& operator =(URL const& url);

              ACE_CString const& get_scheme () const override;

              ACE_CString to_string () const override;

              u_short default_port () const override;

              static char const * PROTOCOL;

              static ACE_CString const& protocol ();

              enum
              {
                FTP_PORT = 21
              };

            protected:
              ACE::INet::ClientRequestHandler* create_default_request_handler () const override;

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
              static Factory const& factory_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/FTP_URL.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_FTP_URL_H */
