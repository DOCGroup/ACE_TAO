// $Id$

#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

/* The ACE_Acceptor<> template lives in the ace/Acceptor.h header
   file. You'll find a very consistent naming convention between the
   ACE objects and the headers where they can be found.  In general,
   the ACE object ACE_Foobar will be found in ace/Foobar.h.  */

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* Since we want to work with sockets, we'll need a SOCK_Acceptor to
   allow the clients to connect to us.  */
#include "ace/SOCK_Acceptor.h"

/* The Client_Handler object we develop will be used to handle clients
   once they're connected.  The ACE_Acceptor<> template's first
   parameter requires such an object.  In some cases, you can get by
   with just a forward declaration on the class, in others you have to
   have the whole thing.  */
#include "client_handler.h"

/* Parameterize the ACE_Acceptor<> such that it will listen for socket
   connection attempts and create Client_Handler objects when they
   happen. In Tutorial 001, we wrote the basic acceptor logic on our
   own before we realized that ACE_Acceptor<> was available.  You'll
   get spoiled using the ACE templates because they take away a lot of
   the tedious details!  */
typedef ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR> Client_Acceptor_Base;

/* Here, we use the parameterized ACE_Acceptor<> as a baseclass for
   our customized Client_Acceptor object.  I've done this so that we
   can provide it with our choice of concurrency strategies when the
   object is created.  Each Client_Handler it creates will use this
   information to determine how to act.  If we were going to create a
   system that was always thread-per-connection, we would not have
   bothered to extend Client_Acceptor.  */
class Client_Acceptor : public Client_Acceptor_Base
{
public:
  /*
    This is always a good idea.  If nothing else, it makes your code more
    orthogonal no matter what baseclasses your objects have.
    */
  typedef Client_Acceptor_Base inherited;

  /*
    Construct the object with the concurrency strategy.  Since this tutorial
    is focused on thread-per-connection, we make that the default.  We could
    have chosen to omitt the default and populate it in main() instead.
    */
  Client_Acceptor (int thread_per_connection = 1)
    : thread_per_connection_ (thread_per_connection)
  {
  }

  /* Return the value of our strategy flag.  This is used by the
    Client_Handler to decide how to act.  If 'true' then the handler
    will behave in a thread-per-connection manner.  */
  int thread_per_connection (void)
  {
    return this->thread_per_connection_;
  }

protected:
  int thread_per_connection_;
};

#endif /* CLIENT_ACCEPTOR_H */
