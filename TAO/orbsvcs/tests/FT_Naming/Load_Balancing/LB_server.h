// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_server.h
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef FT_NAMING_LOAD_BALANCING_LB_SERVER_H
#define FT_NAMING_LOAD_BALANCING_LB_SERVER_H

#include "orbsvcs/FT_NamingManagerC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/Naming/Naming_Client.h"

class Basic;

struct LB_Server_Options
{
  ACE_TString ior_output_file { ACE_TEXT ("obj.ior") };
};

/**
 * @class LB_server
 *
 * @brief Encapsulate the test in class.
 *
 * This is a class used to retrieve the NamingManager reference and
 * create the Object Group.  Servants add themselves to the object
 * group and the object group reference is published to the clients.
 * Clients then invoke the "servant operations" on the NamingManager
 * itself.  At this point the Load Balancing cycle starts.
 */
class LB_server
{
public:
  /// Constructor
  LB_server (int argc, ACE_TCHAR **argv);

  LB_server (LB_server const &) = delete;
  LB_server &operator= (LB_server const &) = delete;
  LB_server (LB_server &&) = delete;
  LB_server &operator= (LB_server &&) = delete;

  /// Destroy the Naming Manager, ORB, and POA.
  int destroy ();

  /// Start the ORB and connect to the naming services.
  int start_orb_and_poa ();

  /// Create the object group using the Naming Manager.
  int create_object_group ();

  /// Register a servant with the object group.
  int register_servant (Basic *servant, char const *loc);

  /// obtain the name service facade
  TAO_Naming_Client &name_svc ();

  /// Return the ORB used by this server.
  CORBA::ORB_ptr orb () const;

  /// Return the current object group reference.
  CORBA::Object_ptr object_group () const;

  /// Return the Naming Manager reference.
  FT_Naming::NamingManager_ptr naming_manager () const;

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);

  int write_ior_to_file (char const *ior);

  /// Naming Manager
  FT_Naming::NamingManager_var naming_manager_;

  /// Object Group.
  CORBA::Object_var object_group_;

  CORBA::ORB_var orb_;
  int argc_;
  // Borrowed from main for ORB_init.
  ACE_TCHAR **argv_;
  PortableServer::POA_var root_poa_;
  LB_Server_Options options_;
  TAO_Naming_Client name_svc_;
};

#endif /* FT_NAMING_LOAD_BALANCING_LB_SERVER_H */
