
//=============================================================================
/**
 *  @file   Server_i.h
 *
 *  The class is the driver for the INS test server.
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================
#ifndef SERVER_I_H
#define SERVER_I_H

#include "tao/Utils/ORB_Manager.h"
#include "INS_i.h"

#include <string>

class Server_i
{
public:
  Server_i () = default;

  ~Server_i () = default;

  Server_i (Server_i const &) = delete;
  Server_i &operator= (Server_i const &) = delete;
  Server_i (Server_i &&) = delete;
  Server_i &operator= (Server_i &&) = delete;

  /// Initialize the Server state - parsing arguments and waiting.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Add an IOR to the ORB-IOR table.
  int add_IOR_to_table (char const *ior, std::string const &ins);

  /// Run the orb.
  int run ();

private:
  /// Servant class
  INS_i servant_;

  /// The ORB manager - a helper class for accessing the POA and
  /// registering objects.
  TAO_ORB_Manager orb_manager_;
};

#endif /* SERVER_I_H */
