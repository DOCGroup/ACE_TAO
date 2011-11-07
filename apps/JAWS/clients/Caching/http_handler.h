/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    http_handler.h
 *
 *  $Id$
 *
 *  @author James Hu
 */
//=============================================================================


#include "ace/SOCK_Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Connector.h"
#include "ace/Svc_Handler.h"

/**
 * @class HTTP_Handler
 *
 * @brief A simple HTTP protocol handler for clients.
 *
 * Checks to see if the requested file is already cached.  If
 * so, it says so.  If not, the request is issued to the
 * connection.  The fetched file is cached.
 */
class HTTP_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  // = Initialization methods.
  HTTP_Handler (void);
  HTTP_Handler (const char * path);

  /// Open hook.
  virtual int open (void *);

  /// Entry points defined by the abstract Svc_Handler.
  virtual int svc (void);

  /// Accessor to the file being fetched.
  const char *filename (void) const;

private:
  char request_[BUFSIZ];
  size_t request_size_;

  char filename_[BUFSIZ];
  size_t response_size_;
};

/**
 * @class HTTP_Connector
 *
 * @brief A simple HTTP connector.
 *
 * Creates an HTTP Handler based on the URL, and then delegates
 * to to the SOCK_CONNECTOR.  Adapter pattern.
 */
class HTTP_Connector
{
public:
  /// User entry point into the HTTP connector.
  int connect (const char * url);

private:
  /// Helper function.
  int parseurl (const char *url,
                char *host,
                u_short *port,
                char *path);

private:
  /// Factory that actively establishes a connection with an HTTP
  /// server.
  ACE_Connector<HTTP_Handler, ACE_SOCK_CONNECTOR> connector_;
};
