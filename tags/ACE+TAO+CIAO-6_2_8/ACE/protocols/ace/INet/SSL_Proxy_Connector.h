// $Id$

/**
 * @file SSL_Proxy_Connector.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_SSL_PROXY_CONNECTOR_H
#define ACE_SSL_PROXY_CONNECTOR_H

#include /**/ "ace/pre.h"

#include "ace/SSL/SSL_SOCK_Stream.h"
#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_SSL_Proxy_Connector
        *
        * @brief Provides the functionality to attach an SSL_SOCK_Stream
        *   to an established socket of a proxy tunneling service.
        *
        * The <connect> method will finalize the setting up an SSL connection
        * over the tunnel by performing an SSL handhake after setting the socket
        * in the @c SSL_SOCK_Stream.
        */
        class SSL_Proxy_Connector
          {
            public:
              SSL_Proxy_Connector ();
              ~SSL_Proxy_Connector ();

              /**
              * Actively connect to an already connected proxy peer, producing a
              * connected @c ACE_SSL_SOCK_Stream object if the connection succeeds.
              * This method uses the provided connection (socket) handle to initialize
              * the @c ACE_SSL_SOCK_Stream object and than finalizes the connection
              * by performing the SSL handshake.
              */
              int connect (ACE_SSL_SOCK_Stream& new_stream,
                           ACE_HANDLE proxy_handle,
                           const ACE_Time_Value *timeout = 0);
            protected:
              /// Complete non-blocking SSL active connection.
              int ssl_connect (ACE_SSL_SOCK_Stream &new_stream,
                              const ACE_Time_Value *timeout);
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_SSL_PROXY_CONNECTOR_H */
