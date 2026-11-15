// -*- C++ -*-

/**
 * @file ior_corbaloc_client_i.h
 * @author Priyanka Gontla <pgontla@ece.uci.edu>
 */

#if !defined (IOR_CORBALOC_CLIENT_I_H)
#define IOR_CORBALOC_CLIENT_I_H

#include "corbalocC.h"
#include "orbsvcs/CosNamingC.h"

#include <string>

/// Sends a corbaloc URL request and checks the server's response.
class IOR_corbaloc_Client_i
{
 public:
  IOR_corbaloc_Client_i () = default;
  ~IOR_corbaloc_Client_i () = default;

  IOR_corbaloc_Client_i (IOR_corbaloc_Client_i const &) = delete;
  IOR_corbaloc_Client_i &operator= (IOR_corbaloc_Client_i const &) = delete;
  IOR_corbaloc_Client_i (IOR_corbaloc_Client_i &&) = delete;
  IOR_corbaloc_Client_i &operator= (IOR_corbaloc_Client_i &&) = delete;

  /// Execute the client example code.
  int run ();

  /// Initialize the client communication endpoint with the server.
  int init (int &argc, ACE_TCHAR *argv[]);

 private:
  /// Corbaloc URL to test.
  std::basic_string<ACE_TCHAR> corbaloc_url_;

  /// Naming context used to resolve the test object.
  CosNaming::NamingContextExt_var naming_context_ {};
};

#endif /* IOR_CORBALOC_CLIENT_I_H */
