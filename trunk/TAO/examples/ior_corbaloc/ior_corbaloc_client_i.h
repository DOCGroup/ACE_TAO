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
#include "orbsvcs/CosNamingC.h"

#include "ace/SString.h"

class IOR_corbaloc_Client_i
{
  // = TITLE
  //     NContextExt Client Implementation
  //
  // = DESCRIPTION
  //

 public:
  // = Constructor and destructor.
  IOR_corbaloc_Client_i (void);
  ~IOR_corbaloc_Client_i (void);

  int run (void);
  // Execute the client example code.

  int init (int& argc, ACE_TCHAR *argv[]);
  // Initialize the client communication endpoint with the server.

 private:
  /// corbaloc url to test
  ACE_TString corbaloc_url_;

  /// Naming context
  CosNaming::NamingContextExt_var naming_context_;
};

#endif /* IOR_CORBALOC_CLIENT_I_H */
