// $Id$

/**
 * @file ClientRequestHandler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_CLIENT_REQUEST_HANDLER_H
#define ACE_INET_CLIENT_REQUEST_HANDLER_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/Thread_Mutex.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/RequestHandler.h"
#include "ace/INet/ConnectionCache.h"
#include <iostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        class URL_Base; // forward

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

              /**
              * @class ACE_INet_INetConnectionKey
              *
              * @brief
              *
              */
#if (defined(_MSC_VER) || defined(__MINGW32__))
              class ACE_INET_Export INetConnectionKey
#else
              class INetConnectionKey
#endif
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

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/ClientRequestHandler.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_CLIENT_REQUEST_HANDLER_H */
