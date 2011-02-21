// $Id$

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
              URL (const ACE_CString& url_string);
              URL (const URL& url);
              virtual ~URL ();

              URL& operator =(const URL& url);

              virtual const ACE_CString& get_scheme () const;

              virtual ACE_CString to_string () const;

              virtual u_short default_port () const;

              static const char* PROTOCOL;

              static const ACE_CString& protocol ();

              enum
              {
                FTP_PORT = 21
              };

            protected:
              virtual ACE::INet::ClientRequestHandler* create_default_request_handler () const;

            private:
              class Factory
                : public ACE::INet::URL_Base::Factory
                {
                  public:
                    Factory ();
                    virtual ~Factory ();
                    virtual const ACE_CString& protocol ();
                    virtual ACE::INet::URL_Base* create_from_string (const ACE_CString& url_string);
                };

              typedef ACE_Singleton<Factory,
                                    ACE_Null_Mutex> TURLFactorySingleton;
              static const Factory& factory_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/FTP_URL.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_FTP_URL_H */
