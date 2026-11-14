// -*- C++ -*-
#ifndef SERVER_I_H
#define SERVER_I_H

#include "orbsvcs/IOR_Multicast.h"

#include <memory>

/// Helper class for the server implementation.
class Server_i
{
public:
  Server_i () = default;
  ~Server_i () = default;

  Server_i (Server_i const&) = delete;
  Server_i (Server_i&&) = delete;
  Server_i& operator= (Server_i const&) = delete;
  Server_i& operator= (Server_i&&) = delete;

  /// Initializes the server multicast.
  int init (int &argc, ACE_TCHAR **argv);

private:
  /// Sets the IOR_Multicast class to listen for multicast requests for this server.
  int enable_multicast (char const *ior);

  /// Parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Number of command-line arguments.
  int argc_ {};

  /// The command-line arguments.
  ACE_TCHAR **argv_ {};

  /// The ORB.
  CORBA::ORB_var orb_ {};

  /// The IOR multicast event handler.
  std::unique_ptr<TAO_IOR_Multicast> ior_multicast_ {};

  /// Multicast address to listen for requests for this server.
  CORBA::String_var mcast_address_ {};
};

#endif /* SERVER_I_H */
