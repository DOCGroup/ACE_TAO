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
#include "INet_Export.h"
#include <iosfwd>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        class ClientRequestHandler; // Forward

        /**
        * @class ACE_INet_URLStream
        *
        * @brief
        *
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
        * @brief Base class for URL/URI addresses conforming to RFC2396.
        *
        */
        class ACE_INET_Export URL_Base
          {
            public:
              URL_Base ();
              virtual ~URL_Base ();

              virtual bool parse (const ACE_CString& url_string) = 0;

              virtual const ACE_CString& get_protocol () const = 0;

              virtual ACE_CString get_authority () const = 0;

              virtual const ACE_CString& get_user_info () const = 0;

              virtual const ACE_CString& get_path () const = 0;

              virtual const ACE_CString& get_query () const = 0;

              virtual URLStream open () const;

              virtual URLStream open (ClientRequestHandler& rh) const;

              virtual ACE_CString to_string () const = 0;

              static URL_Base* create_from_string (const ACE_CString& url_string);

#if defined (ACE_HAS_WCHAR)
              virtual bool parse (const ACE_WString& url_string);

              virtual ACE_WString to_wstring () const;

              static URL_Base* create_from_wstring (const ACE_WString& url_string);
#endif

            protected:
              static const ACE_CString empty_;

              bool strip_protocol (ACE_CString& url_string);

              virtual ClientRequestHandler* create_default_request_handler () const = 0;

            public:
              class Factory
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
                                      ACE_Recursive_Thread_Mutex> TURLFactoryMap;
              typedef ACE_Singleton<TURLFactoryMap,
                                    ACE_Null_Mutex> TURLFactorySingleton;
              static TURLFactoryMap* factories_;
          };

        class ACE_INET_Export URL_INetBase
          : public URL_Base
          {
            public:
              URL_INetBase(u_short port);
              virtual ~URL_INetBase ();

              virtual void set_host (const ACE_CString& host);

              virtual void set_port (u_short port);

              const ACE_CString& get_host () const;

              u_short get_port () const;

            private:
              ACE_CString host_;
              u_short port_;
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "URLBase.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_URL_BASE_H */
