// -*- C++ -*-

//=============================================================================
/**
 *  @file    ior_corbaname_client_i.h
 *
 *  This class implements a simple client which sends a corbaname:
 *  style url to the server and gets a response from the
 *  server to indicate that the server has received the request.
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#if !defined (IOR_CORBANAME_CLIENT_I_H)
#define IOR_CORBANAME_CLIENT_I_H

#include "corbanameC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/SString.h"

/**
 * @class IOR_corbaname_Client_i
 *
 * @brief NContextExt Client Implementation
 */
class IOR_corbaname_Client_i
{
public:
  IOR_corbaname_Client_i () = default;
  IOR_corbaname_Client_i (IOR_corbaname_Client_i const &) = delete;
  IOR_corbaname_Client_i (IOR_corbaname_Client_i &&) = delete;
  IOR_corbaname_Client_i &operator= (IOR_corbaname_Client_i const &) = delete;
  IOR_corbaname_Client_i &operator= (IOR_corbaname_Client_i &&) = delete;
  ~IOR_corbaname_Client_i () = default;

  /// Execute the client example code.
  int run ();

  /// Initialize the client communication endpoint with the server.
  int init (int argc, ACE_TCHAR **argv);

 private:
  CORBA::ORB_var orb_ {};
  ACE_TString name_service_address_ {};
};

#endif /* IOR_CORBANAME_CLIENT_I_H */
