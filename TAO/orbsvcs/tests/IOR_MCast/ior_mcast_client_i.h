// -*- C++ -*-
#if !defined (IOR_MCAST_CLIENT_I_H)
#define IOR_MCAST_CLIENT_I_H

#include "MCastC.h"
class ior_mcast_Client_i
{
  // = TITLE
  //     ior_mcast_client_i.h
  //
  // = DESCRIPTION
  //      Helper class for the client which uses the mcast ior type to
  //      get reference to the server.

public:
  ior_mcast_Client_i () = default;
  ~ior_mcast_Client_i () = default;

  ior_mcast_Client_i (ior_mcast_Client_i const&) = delete;
  ior_mcast_Client_i (ior_mcast_Client_i&&) = delete;
  ior_mcast_Client_i& operator= (ior_mcast_Client_i const&) = delete;
  ior_mcast_Client_i& operator= (ior_mcast_Client_i&&) = delete;

  int init (int &argc, ACE_TCHAR *argv[]);
  // Initialize the client communication endpoint with the server.
};

#endif /* IOR_MCAST_CLIENT_I_H */
