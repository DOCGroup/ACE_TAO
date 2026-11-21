// -*- C++ -*-
#if !defined (IOR_MCAST_CLIENT_I_H)
#define IOR_MCAST_CLIENT_I_H

#include "MCastC.h"
/// Helper class for the client that uses the multicast IOR type to obtain a reference to the server.
class ior_mcast_Client_i
{
public:
  ior_mcast_Client_i () = default;
  ~ior_mcast_Client_i () = default;

  ior_mcast_Client_i (ior_mcast_Client_i const&) = delete;
  ior_mcast_Client_i (ior_mcast_Client_i&&) = delete;
  ior_mcast_Client_i& operator= (ior_mcast_Client_i const&) = delete;
  ior_mcast_Client_i& operator= (ior_mcast_Client_i&&) = delete;

  /// Initializes the client communication endpoint with the server.
  int init (int &argc, ACE_TCHAR *argv[]);
};

#endif /* IOR_MCAST_CLIENT_I_H */
