/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    http_handler.h
 *
 *  @author James Hu
 */
//=============================================================================

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"

#include <string>

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
  HTTP_Handler () = default;
  explicit HTTP_Handler (std::string const &path);

  /// Open hook.
  int open (void *) override;

  /// Entry points defined by the abstract Svc_Handler.
  int svc () override;

  /// Accessor to the file being fetched.
  std::string const &filename () const;

private:
  std::string request_;
  std::string filename_;
  size_t response_size_ {};
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
  int connect (std::string const &url);

private:
  /// Helper function.
  int parseurl (std::string const &url,
                std::string &host,
                u_short &port,
                std::string &path);

private:
  /// Factory that actively establishes a connection with an HTTP
  /// server.
  ACE_Connector<HTTP_Handler, ACE_SOCK_CONNECTOR> connector_;
};
