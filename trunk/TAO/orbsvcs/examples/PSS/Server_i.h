/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/examples/PSS
//
// = DESCRIPTION
//      This class implements the get_status method in server.idl
//
// = AUTHORS
//     Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef SERVER_I_H
#define SERVER_I_H
#include /**/ "ace/pre.h"

#include "ServerS.h"

class Server_i : public virtual POA_Simple_Server::Server
{
 public:

  Server_i ();

  ~Server_i ();

  virtual char *get_status (void);
};

#include /**/ "ace/post.h"
#endif /* SERVER_I_H */

