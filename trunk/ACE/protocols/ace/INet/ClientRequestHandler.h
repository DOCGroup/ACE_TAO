// $Id$

/**
 * @file ClientRequestHandler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_CLIENT_REQUEST_HANDLER_H
#define ACE_INET_CLIENT_REQUEST_HANDLER_H

#include /**/ "ace/pre.h"

#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/Thread_Mutex.h"
#include "INet_Export.h"
#include "URLBase.h"
#include "RequestHandler.h"
#include "ConnectionCache.h"
#include <iostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {

        /**
        * @class ACE_INet_ClientRequestHandler
        *
        * @brief
        *
        */
        class ACE_INET_Export ClientRequestHandler
          : public RequestHandler
          {
            public:
              ClientRequestHandler ();
              virtual ~ClientRequestHandler ();

              virtual std::istream& handle_open_request (const URL_Base& url) = 0;

              virtual std::istream& response_stream () = 0;

              virtual bool is_response_ok () const = 0;

              static ConnectionCache& connection_cache ();

            private:
              typedef ACE_Singleton<ConnectionCache,
                                    ACE_SYNCH_MUTEX> TConnectionCacheSingleton;
          };

        /**
        * @class ACE_INet_ClientINetRequestHandler
        *
        * @brief
        *
        */
        class ACE_INET_Export ClientINetRequestHandler
          : public ClientRequestHandler
          {
            public:
              ClientINetRequestHandler ();
              virtual ~ClientINetRequestHandler ();

            protected:
              /**
              * @class ACE_INet_INetConnectionKey
              *
              * @brief
              *
              */
              class INetConnectionKey
                : public ConnectionKey
                {
                  public:
                    INetConnectionKey (const ACE_CString& host,
                                       u_short port);
                    virtual ~INetConnectionKey ();

                    virtual u_long hash () const;

                    virtual ConnectionKey* duplicate () const;

                    const ACE_CString& host () const;

                    u_short port () const;

                  protected:
                    virtual bool equal (const ConnectionKey& key) const;

                  private:
                    ACE_CString host_;
                    u_short port_;
                };
          };

      };
  };

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ClientRequestHandler.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_CLIENT_REQUEST_HANDLER_H */
