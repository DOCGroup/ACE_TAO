// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_server.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#include "orbsvcs/orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

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

  /// Constructor
  LB_server (int argc, char **argv);

  /// destroys LoadManager, ORB and POA.
  int destroy (void);

  /// start the ORB.
  int start_orb_and_poa (void);

  /// Get the LoadManager Interface.
  int init (int argc, char **argv);

  /// Create the Object Group using the Load Manager Reference.
  int create_object_group (void);

  /// register the servants with the object group.
  int register_servant (Basic *servant, const char *loc);

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb (void);

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr object_group (void);

  /// for servants to get the reference for object_group.
  CosLoadBalancing::LoadManager_ptr load_manager (void);

private:

  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// Object Group.
  CORBA::Object_var object_group_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  /// Used to force the creation of the object group only once.
  static int called_once_;

  /// write the IOR to a file so that it can be read later.
  int write_ior_to_file (const char *);

  CORBA::ORB_var orb_;
  int argc_;
  char **argv_;
  PortableServer::POA_var root_poa_;
};
