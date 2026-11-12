// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_server.h
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef LB_SERVER_H
#define LB_SERVER_H

#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"
#include <string>

// Forward Declaration of the kind of servant we would like to handle
// by the server.
class Basic;

struct LB_Server_Options
{
  // Command-line values borrow argv storage; defaults are string literals.
  ACE_TCHAR const *group_file { ACE_TEXT ("group.ior") };
  ACE_TCHAR const *group_ior { ACE_TEXT ("file://group.ior") };
  ACE_TCHAR const *direct_obj_file { ACE_TEXT ("direct_obj2.ior") };
  int server_id { 1 };
  int pong_delay {};
};

/**
 * @class LB_server
 *
 * @brief Encapsulate the test in class.
 *
 * This is a class used to retrieve the LoadManager reference and
 * create the Object Group.  Servants add themselves to the object
 * group and the object group reference is published to the clients.
 * Clients then invoke the "servant operations" on the LoadManager
 * itself.  At this point the Load Balancing cycle starts.
 */
class LB_server
{
public:
  /// Constructor
  LB_server (int &argc, ACE_TCHAR **&argv, LB_Server_Options &options);

  LB_server (LB_server const &) = delete;
  LB_server &operator= (LB_server const &) = delete;
  LB_server (LB_server &&) = delete;
  LB_server &operator= (LB_server &&) = delete;

  /// destroys LoadManager, ORB and POA.
  int destroy ();

  /// start the ORB.
  int start_orb_and_poa ();

  /// Create the Object Group using the Load Manager Reference.
  int create_object_group ();
  int get_object_group ();

  /// register the servants with the object group.
  int register_servant (Basic *servant, std::string const &location);

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb ();

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr object_group ();

  /// for servants to get the reference for object_group.
  CosLoadBalancing::LoadManager_ptr load_manager ();

private:
  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// Object Group.
  CORBA::Object_var object_group_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  /// write the IOR to a file so that it can be read later.
  int write_ior_to_file (char const *ior);

  CORBA::ORB_var orb_;
  bool object_group_created_ {};
  // ORB_init consumes and updates the caller's argument vector.
  int &argc_;
  ACE_TCHAR **&argv_;
  LB_Server_Options &options_;
  PortableServer::POA_var root_poa_;
};

#endif /* LB_SERVER_H */
