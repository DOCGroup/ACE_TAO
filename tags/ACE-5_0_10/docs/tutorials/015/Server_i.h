// $Id$

#ifndef SERVER_H
#define SERVER_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "Handler.h"

/* Anytime I have templates I try to remember to create a typedef for
   the parameterized object.  It makes for much less typing later!
*/
typedef ACE_Acceptor <Handler, ACE_SOCK_ACCEPTOR> Acceptor;

class Server
{
public:
  // Our simple constructor takes no parameters.  To make the
  // server a bit more useful, you may want to pass in the
  // TCP/IP port to be used by the acceptor.
  Server (void);
  ~Server (void);

  // Open the server for business
  int open (void);

  // Close all server instances by setting the finished_ flag.
  // Actually, the way this class is written, you can only have
  // one instance.
  static int close (void);

  // Run the server's main loop.  The use of the gloabl ACE_Reactor by
  // this method is what limits us to one Server instance.
  int run (void);

private:
  // This will accept client connection requests and instantiate a
  // Handler object for each new connection.
  Acceptor acceptor_;

  // Our shutdown flag
  static sig_atomic_t finished_;
};

#endif /* SERVER_H */
