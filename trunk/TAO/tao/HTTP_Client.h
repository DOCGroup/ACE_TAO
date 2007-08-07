
/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    HTTP_Client.h
 *
 *  $Id$
 *
 *   This is the HTTP_Client class, which is the API for doing file
 *   uploads/downloads.
 *
 *  @author Stoyan Paunov
 */
//=============================================================================


#ifndef TAO_HTTP_CLIENT_H
#define TAO_HTTP_CLIENT_H

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if (TAO_HAS_HTTP_PARSER == 1)

#include "ace/INET_Addr.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Message_Block.h"
#include "tao/HTTP_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_HTTP_Client
 *
 * @brief HTTP_Client is intended to provide application API to
 * classes that wish to do network i/o at a very
 * high level of abstraction.
 *
 * This class provides the ability to retrieve data from
 * the network, of specified length and offset, and potentially
 * use any protocol "under the hood" to do so. It currently
 * uses HTTP. See HTTP_Handler also.
 */
class TAO_Export TAO_HTTP_Client
{
public:
  TAO_HTTP_Client (void);
  ~TAO_HTTP_Client (void);

  /// Initializes the class with the given filename, hostname and port.
  /// it should be called with the filename, before any read/write calls
  int open (const ACE_TCHAR *filename,
            const ACE_TCHAR *hostname = ACE_DEFAULT_SERVER_HOST,
            u_short port = 80);

  /// Starts a connection, and reads a file from the server into
  /// Message_Block mb
  int read (ACE_Message_Block *mb);

  /// Frees memory allocated for filename.
  int close ();

private:
  /// Store the internet address of the server
  ACE_INET_Addr inet_addr_;

  /// The filename
  ACE_TCHAR *filename_;

  /// The connector endpoint to initiate the client connection
  ACE_Connector<TAO_HTTP_Handler, ACE_SOCK_CONNECTOR> connector_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_HTTP_PARSER == 1 */

#endif /* TAO_HTTP_CLIENT_H */
