// $Id$

#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

/* The ACE_Acceptor<> template lives in the ace/Acceptor.h header
   file. You'll find a very consitent naming convention between the
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
typedef ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR> Client_Acceptor;

#endif /* CLIENT_ACCEPTOR_H */
