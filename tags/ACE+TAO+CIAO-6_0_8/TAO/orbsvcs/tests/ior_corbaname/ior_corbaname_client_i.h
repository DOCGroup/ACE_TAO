// -*- C++ -*-

//=============================================================================
/**
 *  @file    ior_corbaname_client_i.h
 *
 *  $Id$
 *
 *  This class implements a simple client which sends a corbaname:
 *  style url to the server and gets a response from the
 *  server to indicate that the server has received the request.
 *
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#if !defined (IOR_CORBANAME_CLIENT_I_H)
#define IOR_CORBANAME_CLIENT_I_H

#include "corbanameC.h"
#include "orbsvcs/CosNamingC.h"

/**
 * @class IOR_corbaname_Client_i
 *
 * @brief NContextExt Client Implementation
 *
 */
class IOR_corbaname_Client_i
{

 public:
  // = Constructor and destructor.
  IOR_corbaname_Client_i (void);
  ~IOR_corbaname_Client_i (void);

  /// Execute the client example code.
  int run (void);

  /// Initialize the client communication endpoint with the server.
  int init (int argc, ACE_TCHAR **argv);

 private:

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// ORB
  CORBA::ORB_var orb_;

  /// Naming context
  CosNaming::NamingContextExt_var naming_context_;
};

#endif /* IOR_CORBANAME_CLIENT_I_H */
