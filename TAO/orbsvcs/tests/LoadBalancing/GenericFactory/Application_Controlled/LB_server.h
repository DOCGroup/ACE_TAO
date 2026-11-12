// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_server.h
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LOADBALANCING_APPLICATION_CONTROLLED_SERVER_H
#define TAO_LOADBALANCING_APPLICATION_CONTROLLED_SERVER_H

#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

#include <string>

// Forward Declaration of the kind of servant we would like to handle
// by the server.
class Basic;

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

  /// Create the Object Group using the Load Manager Reference.
  int create_object_group ();

  /// register the servants with the object group.
  int register_servant (Basic *servant, std::string const &loc);

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb () const;

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr object_group () const;

  /// for servants to get the reference for object_group.
  CosLoadBalancing::LoadManager_ptr load_manager () const;

private:
  int parse_args ();
  int write_ior_to_file (char const *ior) const;

  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// Object Group.
  CORBA::Object_var object_group_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  CORBA::ORB_var orb_;
  int argc_;
  /// Borrowed from main; argv storage must outlive this server.
  ACE_TCHAR **argv_;
  PortableServer::POA_var root_poa_;
  /// The command-line value, when supplied, is borrowed from argv_.
  ACE_TCHAR const *ior_output_file_;
};

#endif /* TAO_LOADBALANCING_APPLICATION_CONTROLLED_SERVER_H */
