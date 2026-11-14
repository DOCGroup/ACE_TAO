// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_server.h
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_LOADBALANCING_MANAGE_OBJECT_GROUP_SERVER_H
#define TAO_LOADBALANCING_MANAGE_OBJECT_GROUP_SERVER_H

#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

#include <string>

// Forward Declaration of the kind of servant we would like to handle
// by the server.
class Basic;
class Simple;

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
  LB_server (int argc, ACE_TCHAR **argv);

  LB_server (LB_server const &) = delete;
  LB_server (LB_server &&) = delete;
  LB_server &operator= (LB_server const &) = delete;
  LB_server &operator= (LB_server &&) = delete;

  /// destroys LoadManager, ORB and POA.
  int destroy ();

  /// start the ORB.
  int start_orb_and_poa ();

  /// Create the basic Object Group using the Load Manager Reference.
  int create_basic_object_group ();

  /// Create the simple Object Group using the Load Manager Reference.
  int create_simple_object_group ();

  /// remove the basic member from the object group
  int remove_basic_member ();

  /// remove the simple member from the object group
  int remove_simple_member ();

  /// register the basic servants with the object group.
  int register_basic_servant (Basic *servant, std::string const &loc);

  /// register the simple servants with the object group.
  int register_simple_servant (Simple *servant, std::string const &loc);

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb ();

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr get_basic_object_group ();

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr get_simple_object_group ();

  /// for servants to get the reference for object_group.
  CosLoadBalancing::LoadManager_ptr load_manager ();

private:
  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// Basic Object Group.
  CORBA::Object_var basic_object_group_;

  /// Simple Object Group.
  CORBA::Object_var simple_object_group_;

  /// factory id for the basic object group.
  PortableGroup::GenericFactory::FactoryCreationId_var basic_fcid_;

  /// factory id for the simple object group.
  PortableGroup::GenericFactory::FactoryCreationId_var simple_fcid_;

  /// ORB pointer
  CORBA::ORB_var orb_;

  /// Borrowed from main; the argument storage must outlive this object.
  int argc_;
  ACE_TCHAR **argv_;

  /// root poa pointer
  PortableServer::POA_var root_poa_;
};

#endif  /* TAO_LOADBALANCING_MANAGE_OBJECT_GROUP_SERVER_H */
