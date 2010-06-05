// $Id$

/**
 * @file HTTP_URL.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_URL_H
#define ACE_HTTP_URL_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "INet_Export.h"
#include "URLBase.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_URL
        *
        * @brief
        *
        */
        class ACE_INET_Export URL
          : public ACE::INet::URL_INetBase
          {
            public:
              URL ();
              URL (const ACE_CString& url_string);
              URL (const URL& url);
              virtual ~URL ();

              URL& operator =(const URL& url);

              virtual bool parse (const ACE_CString& url_string);

#if defined (ACE_HAS_WCHAR)
              virtual bool parse (const ACE_WString& url_string);
#endif

              virtual const ACE_CString& get_protocol () const;

              virtual ACE_CString get_authority () const;

              virtual const ACE_CString& get_user_info () const;

              virtual const ACE_CString& get_path () const;

              virtual const ACE_CString& get_query () const;

              ACE_CString get_request_uri () const;

              void set_user_info (const ACE_CString& userinfo);

              void set_path (const ACE_CString& path);

              void set_query (const ACE_CString& query);

              void set_proxy (const ACE_CString& proxy_host, u_short proxy_port = HTTP_PROXY_PORT);

              const ACE_CString& get_proxy_host () const;

              u_short get_proxy_port () const;

              bool has_proxy () const;

              virtual ACE_CString to_string () const;

              static const ACE_CString PROTOCOL;

              enum
              {
                HTTP_PORT = 80,
                HTTP_PROXY_PORT = 8080
              };

            protected:
              virtual ACE::INet::ClientRequestHandler* create_default_request_handler () const;

            private:
              ACE_CString userinfo_;
              ACE_CString path_;
              ACE_CString query_;
              ACE_CString proxy_host_;
              u_short proxy_port_;

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
      };
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "HTTP_URL.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_URL_H */
