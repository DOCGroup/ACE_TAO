// $Id$

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

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {

        /**
        * @class ACE_INet_URLStream
        *
        * @brief Provides abstracted wrapper class for the resulting
        *   stream of an URL <open> operation.
        *
        * Provides proper life cycle management for either factory provided
        * or user provided request handlers.
        */
        class ACE_INET_Export URLStream
          {
            public:
              URLStream (const URLStream& url_stream);
              ~URLStream ();

              bool operator ! ();

              operator bool ();

              std::istream& operator * ();

              std::istream* operator -> ();

            private:
              friend class URL_Base;

              URLStream (ClientRequestHandler& rh);
              URLStream (ClientRequestHandler* rh);

              typedef ACE_Refcounted_Auto_Ptr<ClientRequestHandler,
                                              ACE_Null_Mutex> TRequestHandlerRef;

              TRequestHandlerRef request_handler_ref_;
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

              virtual bool parse (const ACE_CString& url_string);

              void set_path (const ACE_CString& path);

              virtual void set_query (const ACE_CString& query);

              virtual void set_fragment (const ACE_CString& fragment);

              virtual const ACE_CString& get_scheme () const = 0;

              const ACE_CString& get_protocol () const;

              virtual ACE_CString get_authority () const;

              const ACE_CString& get_path () const;

              virtual const ACE_CString& get_query () const;

              virtual const ACE_CString& get_fragment () const;

              virtual URLStream open () const;

              virtual URLStream open (ClientRequestHandler& rh) const;

              virtual ACE_CString to_string () const = 0;

              static URL_Base* create_from_string (const ACE_CString& url_string);

#if defined (ACE_HAS_WCHAR)
              virtual bool parse (const ACE_WString& url_string);

              virtual ACE_WString to_wstring () const;

              static URL_Base* create_from_wstring (const ACE_WString& url_string);
#endif

              virtual bool validate ();

            protected:
              static const ACE_CString empty_;

              bool strip_scheme (ACE_CString& url_string);

              virtual int parse_authority (std::istream& is);

              virtual bool has_authority ();

              virtual ClientRequestHandler* create_default_request_handler () const = 0;

            private:
              ACE_CString path_;

            public:
#if (_MSC_VER < 1600)
              class ACE_INET_Export Factory
#else
              class Factory
#endif
                {
                  public:
                    Factory ();
                    virtual ~Factory ();
                    virtual const ACE_CString& protocol () = 0;
                    virtual URL_Base* create_from_string (const ACE_CString& url_string) = 0;
                };

              static void register_factory (Factory* url_factory);
              static void deregister_factory (Factory* url_factory);

            private:
              typedef ACE_Map_Manager<ACE_CString,
                                      Factory*,
                                      ACE_SYNCH::MUTEX> TURLFactoryMap;
              typedef ACE_Singleton<TURLFactoryMap,
                                    ACE_SYNCH::NULL_MUTEX> TURLFactorySingleton;
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
              URL_INetBase(u_short port);
              virtual ~URL_INetBase ();

              void set_host (const ACE_CString& host);

              void set_port (u_short port);

              const ACE_CString& get_host () const;

              u_short get_port () const;

              virtual u_short default_port () const = 0;

              virtual ACE_CString get_authority () const;

              virtual bool validate ();

            protected:
              virtual int parse_authority (std::istream& is);

              virtual bool has_authority ();

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
              URL_INetAuthBase(u_short port);
              virtual ~URL_INetAuthBase ();

              const ACE_CString& get_user_info () const;

              void set_user_info (const ACE_CString& userinfo);

              virtual ACE_CString get_authority () const;

            protected:
              virtual int parse_authority (std::istream& is);

            private:
              ACE_CString userinfo_;

            public:
              static bool add_authenticator (const ACE_CString& auth_id, AuthenticatorBase* authenticator);

              static bool has_authenticator (const ACE_CString& auth_id);

              static AuthenticatorBase* remove_authenticator (const ACE_CString& auth_id);

              static bool authenticate (AuthenticationBase& authentication);

            private:
              typedef ACE_Refcounted_Auto_Ptr<AuthenticatorBase,
                                              ACE_SYNCH::NULL_MUTEX> authenticator_ptr;
              typedef ACE_Map_Manager<ACE_CString,
                                      authenticator_ptr,
                                      ACE_SYNCH::RECURSIVE_MUTEX>  authenticator_map;

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
