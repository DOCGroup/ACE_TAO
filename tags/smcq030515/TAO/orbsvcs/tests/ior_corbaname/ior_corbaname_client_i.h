// -*- C++ -*-
// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/tests/ior_corbaname/
//
// = FILENAME
//    ior_corbaname_client_i.h
//
// = DESCRIPTION
//    This class implements a simple client which sends a corbaname:
//    style url to the server and gets a response from the
//    server to indicate that the server has received the request.
//
// = AUTHORS
//    Priyanka Gontla <pgontla@ece.uci.edu>
//
//============================================================


#if !defined (IOR_CORBANAME_CLIENT_I_H)
#define IOR_CORBANAME_CLIENT_I_H

#include "corbanameC.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

class IOR_corbaname_Client_i
{
  // = TITLE
  //     NContextExt Client Implementation
  //
  // = DESCRIPTION
  //

 public:
  // = COnstructor and destructor.
  IOR_corbaname_Client_i (void);
  ~IOR_corbaname_Client_i (void);

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Execute the client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with the server.

 private:

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  CORBA::ORB_var orb_;
  // ORB

  CosNaming::NamingContextExt_var naming_context_;
  // Naming context

};

#endif /* IOR_CORBANAME_CLIENT_I_H */
