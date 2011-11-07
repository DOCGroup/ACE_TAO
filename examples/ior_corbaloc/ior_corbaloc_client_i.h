// -*- C++ -*-

//=============================================================================
/**
 *  @file    ior_corbaloc_client_i.h
 *
 *  $Id$
 *
 *  This class implements a simple client which sends a corbaloc:
 *  style url to the server and gets a response from the
 *  server to indicate that the server has received the request.
 *
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#if !defined (IOR_CORBALOC_CLIENT_I_H)
#define IOR_CORBALOC_CLIENT_I_H

#include "corbalocC.h"
#include "orbsvcs/CosNamingC.h"

#include "ace/SString.h"

/**
 * @class IOR_corbaloc_Client_i
 *
 * @brief NContextExt Client Implementation
 *
 */
class IOR_corbaloc_Client_i
{

 public:
  // = Constructor and destructor.
  IOR_corbaloc_Client_i (void);
  ~IOR_corbaloc_Client_i (void);

  /// Execute the client example code.
  int run (void);

  /// Initialize the client communication endpoint with the server.
  int init (int& argc, ACE_TCHAR *argv[]);

 private:
  /// corbaloc url to test
  ACE_TString corbaloc_url_;

  /// Naming context
  CosNaming::NamingContextExt_var naming_context_;
};

#endif /* IOR_CORBALOC_CLIENT_I_H */
