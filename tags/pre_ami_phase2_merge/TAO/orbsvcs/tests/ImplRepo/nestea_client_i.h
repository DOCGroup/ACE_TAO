// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
//
// = FILENAME
//    nestea_client_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client which controls a Nestea
//    can database
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (NESTEA_CLIENT_I_H)
#define NESTEA_CLIENT_I_H

#include "tao/corba.h"
#include "NesteaC.h"

class Nestea_Client_i
{
  // = TITLE
  //     Nestea Bookshelf Client Implementation
  //
  // = DESCRIPTION
  //     Class wrapper for a client which gets the server IOR and then makes
  //     a couple of calls to the server.
public:
  // = Constructor and destructor.
  Nestea_Client_i (void);
  ~Nestea_Client_i (void);

  int run ();
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  int parse_args (void);
  // Parses the arguments passed on the command line.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *server_key_;
  // Key of the obj ref of the server.

  Nestea_Bookshelf_ptr server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // Remember our orb.
};

#endif /* NESTEA_CLIENT_I_H */
