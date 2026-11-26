/**
 * @file URLBase.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_URL_BASE_H
#define ACE_INET_URL_BASE_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Map_Manager.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/AuthenticationBase.h"
#include "ace/INet/ClientRequestHandler.h"
#include <iosfwd>
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_URLStream
        *
        * @brief Provides abstracted wrapper class for the resulting
        *   stream of an URL @c open operation.
        *
        * Provides proper life cycle management for either factory provided
        * or user provided request handlers.
        */
        class ACE_INET_Export URLStream
          {
            public:
              URLStream (URLStream const& url_stream);
              ~URLStream ();

              bool operator ! ();

              explicit operator bool ();

              std::istream& operator * ();

              std::istream* operator -> ();

            private:
              friend class URL_Base;

              explicit URLStream (ClientRequestHandler& rh);
              explicit URLStream (ClientRequestHandler* rh);

              /// Owns handlers created by open().
              std::shared_ptr<ClientRequestHandler> request_handler_ref_;
              /// May observe a caller-owned handler.
              ClientRequestHandler* request_handler_;
          };

        /**
        * @class ACE_INet_URL_Base
        *
        * @brief Base class for URL/URI addresses conforming to RFC3986.
        *
        */
        class ACE_INET_Export URL_Base
          {
            public:
              URL_Base ();
              virtual ~URL_Base ();

              virtual bool parse (ACE_CString const& url_string);

              void set_path (ACE_CString const& path);

              virtual void set_query (ACE_CString const& query);

              virtual void set_fragment (ACE_CString const& fragment);

              virtual ACE_CString const& get_scheme () const = 0;

              ACE_CString const& get_protocol () const;

              virtual ACE_CString get_authority () const;

              ACE_CString const& get_path () const;

              virtual ACE_CString const& get_query () const;

              virtual ACE_CString const& get_fragment () const;

              virtual URLStream open () const;

              virtual URLStream open (ClientRequestHandler& rh) const;

              virtual ACE_CString to_string () const = 0;

              static URL_Base* create_from_string (ACE_CString const& url_string);

#if defined (ACE_HAS_WCHAR)
              virtual bool parse (ACE_WString const& url_string);

              virtual ACE_WString to_wstring () const;

              static URL_Base* create_from_wstring (ACE_WString const& url_string);
#endif

              virtual bool validate ();

            protected:
              static ACE_CString const empty_;

              bool strip_scheme (ACE_CString& url_string);

              virtual int parse_authority (std::istream& is);

              virtual bool has_authority ();

              virtual ClientRequestHandler* create_default_request_handler () const = 0;

            private:
              ACE_CString path_;

            public:
              class ACE_INET_Export Factory
                {
                  public:
                    Factory ();
                    virtual ~Factory ();
                    virtual ACE_CString const& protocol () = 0;
                    virtual URL_Base* create_from_string (ACE_CString const& url_string) = 0;
                };

              static void register_factory (Factory* url_factory);
              static void deregister_factory (Factory* url_factory);

            private:
              using TURLFactoryMap = ACE_Map_Manager<ACE_CString, Factory*, ACE_SYNCH::MUTEX>;
              using TURLFactorySingleton = ACE_Singleton<TURLFactoryMap, ACE_SYNCH::NULL_MUTEX>;
              static TURLFactoryMap* factories_;
          };

        /**
        * @class ACE_INet_URL_INetBase
        *
        * @brief Base class for internet protocol based URL/URI
        *        addresses supporting host(name) and port specification.
        *
        */
        class ACE_INET_Export URL_INetBase
          : public URL_Base
          {
            public:
              explicit URL_INetBase(u_short port);
              virtual ~URL_INetBase ();

              void set_host (ACE_CString const& host);

              void set_port (u_short port);

              ACE_CString const& get_host () const;

              u_short get_port () const;

              virtual u_short default_port () const = 0;

              ACE_CString get_authority () const override;

              bool validate () override;

            protected:
              int parse_authority (std::istream& is) override;

              bool has_authority () override;

              int parse_authority_i (std::istream& is,
                                     std::ostream& os,
                                     int lastch);

            private:
              ACE_CString host_;
              u_short port_;
          };

        /**
        * @class ACE_INet_URL_INetAuthBase
        *
        * @brief Base class for internet protocol based URL/URI
        *        addresses supporting authentication information.
        *
        * This implementation does *not* support the specification
        * of passwords in user_info field of the URL (deprecated as
        * of RFC3986).
        */
        class ACE_INET_Export URL_INetAuthBase
          : public URL_INetBase
          {
            public:
              explicit URL_INetAuthBase(u_short port);
              virtual ~URL_INetAuthBase ();

              ACE_CString const& get_user_info () const;

              void set_user_info (ACE_CString const& userinfo);

              ACE_CString get_authority () const override;

            protected:
              int parse_authority (std::istream& is) override;

            private:
              ACE_CString userinfo_;

            public:
              static bool add_authenticator (ACE_CString const& auth_id, AuthenticatorBase* authenticator);

              static bool has_authenticator (ACE_CString const& auth_id);

              static AuthenticatorBase* remove_authenticator (ACE_CString const& auth_id);

              static bool authenticate (AuthenticationBase& authentication);

            private:
              using authenticator_ptr = ACE_Refcounted_Auto_Ptr<AuthenticatorBase, ACE_SYNCH::NULL_MUTEX>;
              using authenticator_map = ACE_Map_Manager<ACE_CString, authenticator_ptr, ACE_SYNCH::RECURSIVE_MUTEX>;

              static authenticator_map authenticators_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/URLBase.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_URL_BASE_H */
