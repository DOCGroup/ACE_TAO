// $Id$

#ifndef CLIENT_H
#define CLIENT_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Protocol_Stream.h"

class ACE_Message_Block;

/* Hide the details of connection and protocol-conformance from the
   application-level logic.
*/
class Client
{
public:
  // Provide the server information when constructing the
  // object.  This could (and probably should) be moved to the
  // open() method.
  Client (u_short port,
          const char *server);

  // Cleanup...
  ~Client (void);

  // Open the connection to the server.
  int open (void);

  // Close the connection to the server.  Be sure to do this
  // before you let the Client go out of scope.
  int close (void);

  // Put a message to the server.  The Client assumes ownership of
  // <message> at that point and will release() it when done.  Do not
  // use <message> after passing it to put().
  int put (ACE_Message_Block *message);

  // Get a response from the server.  The caller becomes the owner of
  // <response> after this call and is responsible for invoking
  // release() when done.
  int get (ACE_Message_Block *&response);

private:
  // Protocol_Stream hides the protocol conformance details from us.
  Protocol_Stream stream_;

  // We create a connection on the peer_ and then pass ownership of it
  // to the protocol stream.
  ACE_SOCK_Stream peer_;

  // Endpoing information saved by the constructor for use by open().
  u_short port_;
  const char *server_;

  // Accessors for the complex member variables.

  Protocol_Stream &stream (void)
  {
    return this->stream_;
  }

  ACE_SOCK_Stream &peer (void)
  {
    return this->peer_;
  }
};

#endif /* CLIENT_H */
