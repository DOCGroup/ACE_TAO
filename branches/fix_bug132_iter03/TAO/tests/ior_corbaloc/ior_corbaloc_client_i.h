// -*- C++ -*-
// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/tests/ior_corbaloc/
//
// = FILENAME
//    ior_corbaloc_client_i.h
//
// = DESCRIPTION
//    This class implements a simple client which sends a corbaloc:
//    style url to the server and gets a response from the
//    server to indicate that the server has received the request.
//
// = AUTHORS
//    Priyanka Gontla <pgontla@ece.uci.edu>
//
//============================================================


#if !defined (IOR_CORBALOC_CLIENT_I_H)
#define IOR_CORBALOC_CLIENT_I_H

#include "corbalocC.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

class IOR_corbaloc_Client_i
{
  // = TITLE
  //     NContextExt Client Implementation
  //
  // = DESCRIPTION
  //

 public:
  // = COnstructor and destructor.
  IOR_corbaloc_Client_i (void);
  ~IOR_corbaloc_Client_i (void);

  int run (CORBA::Environment &ACE_TRY_ENV);
  // Execute the client example code.

  int init (int argc, char **argv, CORBA::Environment &ACE_TRY_ENV);
  // Initialize the client communication endpoint with the server.

 private:

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  CosNaming::NamingContextExt_var naming_context_;
  // Naming context

};

#endif /* IOR_CORBALOC_CLIENT_I_H */
