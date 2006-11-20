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


#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

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

  /// Constructor
  LB_server (int argc, char **argv);

  /// destroys LoadManager, ORB and POA.
  int destroy (void);

  /// start the ORB.
  int start_orb_and_poa (void);

  /// Get the LoadManager Interface.
  int init (int argc, char **argv);

  /// Create the basic Object Group using the Load Manager Reference.
  int create_basic_object_group (void);

  /// Create the simple Object Group using the Load Manager Reference.
  int create_simple_object_group (void);

  /// remove the basic member from the object group
  int remove_basic_member (void);

  /// remove the simple member from the object group
  int remove_simple_member (void);

  /// register the basic servants with the object group.
  int register_basic_servant (Basic *servant, const char *loc);

  /// register the simple servants with the object group.
  int register_simple_servant (Simple *servant, const char *loc);

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb (void);

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr get_basic_object_group (void);

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr get_simple_object_group (void);

  /// for servants to get the reference for object_group.
  CosLoadBalancing::LoadManager_ptr load_manager (void);

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

  /// command line arguments
  int argc_;
  char **argv_;

  /// root poa pointer
  PortableServer::POA_var root_poa_;
};
